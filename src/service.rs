use serde::{Deserialize, Serialize};
use std::fs::File;
use std::io::BufReader;
use std::path::Path;

use ibm_quantum_platform_api::apis::backends_api::{
    get_backend_configuration, get_backend_properties, list_backends,
};
use ibm_quantum_platform_api::apis::jobs_api::{create_job, get_job_details_jid};
use ibm_quantum_platform_api::models::CreateJobRequest;
use ibmcloud_global_search_api::apis::configuration::Configuration as SearchConfiguration;
use ibmcloud_global_search_api::apis::search_api::search;
use ibmcloud_iam_api::apis::configuration::Configuration;
use ibmcloud_iam_api::apis::token_operations_api::get_token_api_key;
use ibmcloud_iam_api::models::token_response::TokenResponse;

use std::collections::HashMap;
use std::error;
use std::fmt::{Debug, Display, Formatter};
use ibm_quantum_platform_api::models;
use ibm_quantum_platform_api::models::backends_response_v2_devices_inner_status::Name;
use ibm_quantum_platform_api::models::job_response::Status;
use crate::ExitCode;

#[derive(Deserialize, Serialize, Clone)]
pub struct AccountEntry {
    pub channel: String,
    pub instance: Option<String>,
    #[serde(default)]
    pub private_endpoint: bool,
    pub token: String,
    pub url: String,
    proxies: Option<ProxyConfiguration>,
    #[serde(default)]
    pub verify: bool,
}

#[derive(Deserialize, Serialize, Clone)]
struct ProxyConfiguration {
    urls: Option<HashMap<String, String>>,
    username_ntlm: Option<String>,
    password_ntlm: Option<String>,
}

#[derive(Clone, Debug)]
pub struct Job {
    id: String,
    crn: String, // The CRN used when submitting the job
}

#[derive(Clone, Debug)]
pub struct JobDetails(models::JobResponse);

#[repr(u32)]
pub enum JobStatus {
    Queued = 0,
    Running = 1,
    Completed = 2,
    Cancelled = 3,
    CancelledRanTooLong = 4,
    Failed = 5,
}

impl JobDetails {
    fn status(&self) -> JobStatus {
        match self.0.status {
            Status::Queued => JobStatus::Queued,
            Status::Running => JobStatus::Running,
            Status::Completed => JobStatus::Completed,
            Status::Cancelled => JobStatus::Cancelled,
            Status::CancelledRanTooLong => JobStatus::CancelledRanTooLong,
            Status::Failed => JobStatus::Failed,
        }
    }
}

#[derive(Clone, Debug)]
pub struct ServiceError {
    code: ExitCode,
    message: String,
}

impl ServiceError {
    pub fn code(&self) -> ExitCode {
        self.code
    }

    pub fn message(&self) -> &str {
        &self.message
    }
}

impl Display for ServiceError {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.message)
    }
}

impl error::Error for ServiceError {}

impl<T: Debug> From<ibm_quantum_platform_api::apis::Error<T>> for ServiceError {
    fn from(value: ibm_quantum_platform_api::apis::Error<T>) -> Self {
        match value {
            ibm_quantum_platform_api::apis::Error::ResponseError(e) => {
                ServiceError {
                    code: match e.status.as_u16() {
                        400 => ExitCode::QuantumAPIBadRequest,
                        401 => ExitCode::QuantumAPIUnauthenticated,
                        403 => ExitCode::QuantumAPIForbidden,
                        404 => ExitCode::QuantumAPINotFound,
                        409 => ExitCode::QuantumAPIConflict,
                        _ => ExitCode::QuantumAPIUnhandledError,
                    },
                    message: e.content,
                }
            }
            _ => ServiceError {
                code: ExitCode::QuantumAPIUnhandledError,
                message: format!("Unhandled Qiskit runtime API error:\n\n{:?}", value).to_string(),
            }
        }
    }
}

impl<T: Debug> From<ibmcloud_global_search_api::apis::Error<T>> for ServiceError {
    fn from(value: ibmcloud_global_search_api::apis::Error<T>) -> Self {
        match value {
            ibmcloud_global_search_api::apis::Error::ResponseError(e) => {
                ServiceError {
                    code: match e.status.as_u16() {
                        400 => ExitCode::GlobalSearchAPIBadRequest,
                        401 => ExitCode::GlobalSearchAPIUnauthenticated,
                        403 => ExitCode::GlobalSearchAPIForbidden,
                        404 => ExitCode::GlobalSearchAPINotFound,
                        409 => ExitCode::GlobalSearchAPIConflict,
                        _ => ExitCode::GlobalSearchAPIUnhandledError,
                    },
                    message: e.content,
                }
            }
            _ => ServiceError {
                code: ExitCode::GlobalSearchAPIUnhandledError,
                message: format!("Unhandled IBM Global Search API error:\n\n{:?}", value).to_string(),
            }
        }
    }
}

impl<T: Debug> From<ibmcloud_iam_api::apis::Error<T>> for ServiceError {
    fn from(value: ibmcloud_iam_api::apis::Error<T>) -> Self {
        match value {
            ibmcloud_iam_api::apis::Error::ResponseError(e) => {
                ServiceError {
                    code: match e.status.as_u16() {
                        400 => ExitCode::IAMAPIBadRequest,
                        401 => ExitCode::IAMAPIUnauthenticated,
                        403 => ExitCode::IAMAPIForbidden,
                        404 => ExitCode::IAMAPINotFound,
                        409 => ExitCode::IAMAPIConflict,
                        _ => ExitCode::IAMAPIUnhandledError,
                    },
                    message: e.content,
                }
            }
            _ => ServiceError {
                code: ExitCode::IAMAPIUnhandledError,
                message: format!("Unhandled IBM IAM API error:\n\n{:?}", value).to_string(),
            }
        }
    }
}

fn get_account_config(filename: Option<&str>, name: Option<&str>) -> AccountEntry {
    let filename = match filename {
        Some(path) => path.to_string(),
        None => {
            let home = std::env::var("HOME").unwrap();
            format!("{}/.qiskit/qiskit-ibm.json", home)
        }
    };
    let file_path = Path::new(&filename);

    let file = File::open(file_path).unwrap();
    let reader = BufReader::new(file);
    let accounts: HashMap<String, AccountEntry> = serde_json::from_reader(reader).unwrap();
    match name {
        Some(name) => accounts[name].clone(),
        None => accounts
            .get("default")
            .or_else(|| accounts.get("default-ibm-quantum-platform"))
            .unwrap_or_else(|| &accounts["default-ibm-cloud"])
            .clone(),
    }
}

#[derive(Clone)]
pub struct Account {
    pub config: AccountEntry,
    token: TokenResponse,
    iam_config: Configuration,
}

impl Account {
    pub fn get_access_token(&self) -> Option<&str> {
        self.token.access_token.as_deref()
    }
}

pub async fn get_account_from_config(filename: Option<&str>, name: Option<&str>) -> Result<Account, ServiceError> {
    let config = get_account_config(filename, name);
    let iam_config = Configuration {
        base_path: "https://iam.cloud.ibm.com".to_owned(),
        user_agent: Some("qiskit-ibm-runtime-rs/0.0.1".to_owned()),
        client: reqwest::Client::new(),
        basic_auth: None,
        oauth_access_token: None,
        bearer_access_token: None,
        api_key: None,
    };
    let response = get_token_api_key(
        &iam_config,
        "urn:ibm:params:oauth:grant-type:apikey",
        config.token.as_str(),
        None,
    )
    .await?;
    Ok(Account {
        config,
        token: response,
        iam_config,
    })
}

pub async fn list_instances(account: &Account) -> Vec<String> {
    let mut config = SearchConfiguration::default();
    config.user_agent = Some("qiskit-ibm-runtime-rs/0.0.1".to_string());
    config.api_key = Some(ibmcloud_global_search_api::apis::configuration::ApiKey {
        key: account.get_access_token().unwrap().to_string(),
        prefix: Some("Bearer".to_string()),
    });
    let body = ibmcloud_global_search_api::models::SearchRequest::FirstCall(Box::new(
        ibmcloud_global_search_api::models::FirstCall {
            query: "service_name:quantum-computing".to_string(),
            fields: Some(
                ["crn", "service_plan_unique_id", "name", "doc"]
                    .into_iter()
                    .map(|x| x.to_string())
                    .collect(),
            ),
        },
    ));
    let resp = search(
        &config,
        body,
        None,      // x_request_id
        None,      // x_correlation_id
        None,      // account_id
        Some(100), // limit
        None,      // timeout
        None,      // sort
        None,      // is_deleted
        None,      // is_reclaimed
        None,      // is_public
        None,      // impersonate_user
        None,      // can_tag
        None,      // is_project_resource
    )
    .await;
    let items = resp.unwrap().items;
    println!("items: {:?}", items);
    items.into_iter().map(|x| x.crn).collect()
}

pub async fn get_backend(account: Account, backend: &str) -> crate::qiskit_target::Target {
    let mut config = ibm_quantum_platform_api::apis::configuration::Configuration::default();
    config.user_agent = Some("qiskit-ibm-runtime-rs/0.0.1".to_string());
    config.api_key = Some(ibm_quantum_platform_api::apis::configuration::ApiKey {
        key: account.get_access_token().unwrap().to_string(),
        prefix: Some("Bearer".to_string()),
    });
    let crns = list_instances(&account).await;
    //    let crn = crns[0].as_str();
    let backend_configuration = get_backend_configuration(&config, backend, Some("2025-06-01"))
        .await
        .unwrap();
    let backend_properties = get_backend_properties(&config, backend, Some("2025-06-01"), None)
        .await
        .unwrap();
    let num_qubits = backend_configuration["n_qubits"]
        .as_u64()
        .unwrap()
        .try_into()
        .unwrap();
    let mut gate_prop_map: HashMap<String, Vec<(Vec<u32>, Option<f64>, Option<f64>)>> =
        HashMap::new();
    for gate in backend_properties["gates"].as_array().unwrap() {
        let gate_map = gate.as_object().unwrap();
        let gate_params = gate_map["parameters"].as_array().unwrap();
        let mut gate_error: Option<f64> = None;
        let mut gate_duration: Option<f64> = None;
        for prop in gate_params {
            let prop = prop.as_object().unwrap();
            let name = prop["name"].as_str().unwrap();
            if name == "gate_error" {
                gate_error = prop["value"].as_f64();
            } else if name == "gate_length" {
                let unit = prop["unit"].as_str().unwrap();
                if unit == "ns" {
                    gate_duration = prop["value"].as_f64().map(|x| x * 1e-9);
                } else {
                    panic!("Non-nanosecond unit for gate duration");
                }
            }
        }
        let qargs = gate_map["qubits"]
            .as_array()
            .unwrap()
            .iter()
            .map(|x| x.as_u64().unwrap() as u32)
            .collect::<Vec<_>>();
        gate_prop_map
            .entry(gate_map["gate"].as_str().unwrap().to_string())
            .and_modify(|props| {
                props.push((qargs.clone(), gate_duration, gate_error));
            })
            .or_insert(vec![(qargs, gate_duration, gate_error)]);
    }
    let mut target = crate::qiskit_target::Target::new(num_qubits);
    for (gate, props) in gate_prop_map {
        let gate = if gate == "x" {
            crate::qiskit_target::ISAGate::X
        } else if gate == "sx" {
            crate::qiskit_target::ISAGate::SX
        } else if gate == "cz" {
            crate::qiskit_target::ISAGate::CZ
        } else if gate == "ecr" {
            crate::qiskit_target::ISAGate::ECR
        } else if gate == "rz" {
            crate::qiskit_target::ISAGate::RZ
        } else if gate == "rx" {
            crate::qiskit_target::ISAGate::RX
        } else if gate == "id" {
            crate::qiskit_target::ISAGate::I
        } else if gate == "cx" {
            crate::qiskit_target::ISAGate::CX
        } else {
            panic!("What is a {gate}");
        };
        target.add_gate(
            gate,
            props
                .into_iter()
                .map(|(qubits, dur, err)| (qubits, [dur, err])),
        );
    }
    target
}

pub async fn submit_sampler_job(
    account: Account,
    crn: &str,
    backend: String,
    circuit: &crate::qiskit_circuit::Circuit,
    shots: Option<i32>,
    runtime: Option<String>,
    tags: Option<Vec<String>>,
) -> Result<Job, ServiceError> {
    let mut config = ibm_quantum_platform_api::apis::configuration::Configuration::default();
    config.user_agent = Some("qiskit-ibm-runtime-rs/0.0.1".to_string());
    config.api_key = Some(ibm_quantum_platform_api::apis::configuration::ApiKey {
        key: account.get_access_token().unwrap().to_string(),
        prefix: Some("Bearer".to_string()),
    });
    let job_payload = crate::generate_job_params::create_sampler_job_payload(
        circuit,
        backend.clone(),
        shots,
        runtime,
        tags,
    );
    let file = File::create("/tmp/test.json").unwrap();
    serde_json::to_writer_pretty(file, &job_payload).unwrap();
    let res = create_job(
        &config,
        crn,
        Some("2025-06-01"),
        None,
        Some(CreateJobRequest::CreateJobRequestOneOf(Box::new(
            job_payload,
        ))),
    )
    .await?;
    println!("result: {:?}", res);
    Ok(Job {
        id: res.id,
        crn: crn.to_string(),
    })
}

pub async fn get_job_details(account: Account, job: &Job) -> Result<JobDetails, ServiceError> {
    let mut config = ibm_quantum_platform_api::apis::configuration::Configuration::default();
    config.user_agent = Some("qiskit-ibm-runtime-rs/0.0.1".to_string());
    config.api_key = Some(ibm_quantum_platform_api::apis::configuration::ApiKey {
        key: account.get_access_token().unwrap().to_string(),
        prefix: Some("Bearer".to_string()),
    });
    let details = get_job_details_jid(&config, job.crn.as_str(), job.id.as_str(), Some("2025-06-01"), None).await?;
    println!("details: {:?}", details);
    Ok(JobDetails(details))
}

pub async fn get_job_status(account: Account, job: &Job) -> Result<JobStatus, ServiceError> {
    let details = get_job_details(account, job).await?;
    Ok(details.status())
}

pub async fn get_backends(account: Account, instance_crn: &str) -> Vec<String> {
    let mut config = ibm_quantum_platform_api::apis::configuration::Configuration::default();
    config.user_agent = Some("qiskit-ibm-runtime-rs/0.0.1".to_string());
    config.api_key = Some(ibm_quantum_platform_api::apis::configuration::ApiKey {
        key: account.get_access_token().unwrap().to_string(),
        prefix: Some("Bearer".to_string()),
    });
    let resp = list_backends(&config, Some("2025-06-01"), instance_crn).await;
    resp.unwrap()
        .devices
        .unwrap()
        .into_iter()
        .filter(|d| matches!(d.status.name, Name::Online))
        .map(|x| x.name)
        .collect()
}
