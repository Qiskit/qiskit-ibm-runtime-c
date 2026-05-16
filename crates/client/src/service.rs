// This code is part of Qiskit.
//
// (C) Copyright IBM 2025
//
// This code is licensed under the Apache License, Version 2.0. You may
// obtain a copy of this license in the LICENSE.txt file in the root directory
// of this source tree or at http://www.apache.org/licenses/LICENSE-2.0.
//
// Any modifications or derivative works of this code must retain this
// copyright notice, and modified files need to carry a notice indicating
// that they have been altered from the originals.

use serde::{Deserialize, Serialize};
use std::fs::File;
use std::io::BufReader;
use std::path::Path;

use ibm_quantum_platform_api::apis::backends_api::{
    get_backend_configuration, get_backend_properties, list_backends,
};
use ibm_quantum_platform_api::apis::jobs_api::{
    create_job, get_estimator_job_results_jid, get_job_details_jid, get_sampler_job_results_jid,
};
use ibm_quantum_platform_api::models::{
    BackendsResponseV2DevicesInner, CreateJob200Response, CreateJobRequest,
};
use ibmcloud_global_search_api::apis::configuration::Configuration as SearchConfiguration;
use ibmcloud_global_search_api::apis::search_api::search;
use ibmcloud_iam_api::apis::configuration::Configuration;
use ibmcloud_iam_api::apis::token_operations_api::get_token_api_key;
use ibmcloud_iam_api::models::token_response::TokenResponse;

use crate::{log_debug, log_warn, ExitCode};
use ibm_quantum_platform_api::models;
use ibm_quantum_platform_api::models::job_response::Status;
use std::collections::HashMap;
use std::error;
use std::ffi::{c_char, CString};
use std::fmt::{Debug, Display, Formatter};

#[derive(Deserialize, Serialize, Clone, Debug)]
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

#[derive(Deserialize, Serialize, Clone, Debug)]
struct ProxyConfiguration {
    urls: Option<HashMap<String, String>>,
    username_ntlm: Option<String>,
    password_ntlm: Option<String>,
}

#[derive(Clone, Debug)]
pub struct Job {
    instance: Instance,
    response: CreateJob200Response,
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

    pub fn job_id(&self) -> &str {
        &self.0.id
    }
}

#[derive(Clone, Debug)]
pub struct Backend {
    pub(crate) name: CString,
    instance: Instance,
    response: BackendsResponseV2DevicesInner,
}

impl Backend {
    pub fn name(&self) -> *const c_char {
        self.name.as_ptr()
    }

    pub fn instance_name(&self) -> *const c_char {
        self.instance.name.as_ptr()
    }

    pub fn instance_crn(&self) -> *const c_char {
        self.instance.crn.as_ptr()
    }
}

// Note: this cannot simply derive Clone since the ptrs cache would be wrong
#[derive(Debug)]
pub struct BackendSearchResults {
    backends: Vec<Box<Backend>>,
    ptrs: Vec<*const Backend>,
}

impl BackendSearchResults {
    pub fn data_ptr(&self) -> *const *const Backend {
        self.ptrs.as_ptr()
    }

    pub fn len(&self) -> usize {
        self.ptrs.len()
    }

    pub fn least_busy(&self) -> *const Backend {
        self.backends
            .iter()
            .min_by_key(|b| b.response.queue_length)
            .map(|b| b.as_ref() as *const Backend)
            .unwrap_or(std::ptr::null())
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
            ibm_quantum_platform_api::apis::Error::ResponseError(e) => ServiceError {
                code: match e.status.as_u16() {
                    400 => ExitCode::QuantumAPIBadRequest,
                    401 => ExitCode::QuantumAPIUnauthenticated,
                    403 => ExitCode::QuantumAPIForbidden,
                    404 => ExitCode::QuantumAPINotFound,
                    409 => ExitCode::QuantumAPIConflict,
                    _ => ExitCode::QuantumAPIUnhandledError,
                },
                message: e.content,
            },
            _ => ServiceError {
                code: ExitCode::QuantumAPIUnhandledError,
                message: format!("Unhandled Qiskit runtime API error:\n\n{:?}", value).to_string(),
            },
        }
    }
}

impl<T: Debug> From<ibmcloud_global_search_api::apis::Error<T>> for ServiceError {
    fn from(value: ibmcloud_global_search_api::apis::Error<T>) -> Self {
        match value {
            ibmcloud_global_search_api::apis::Error::ResponseError(e) => ServiceError {
                code: match e.status.as_u16() {
                    400 => ExitCode::GlobalSearchAPIBadRequest,
                    401 => ExitCode::GlobalSearchAPIUnauthenticated,
                    403 => ExitCode::GlobalSearchAPIForbidden,
                    404 => ExitCode::GlobalSearchAPINotFound,
                    409 => ExitCode::GlobalSearchAPIConflict,
                    _ => ExitCode::GlobalSearchAPIUnhandledError,
                },
                message: e.content,
            },
            _ => ServiceError {
                code: ExitCode::GlobalSearchAPIUnhandledError,
                message: format!("Unhandled IBM Global Search API error:\n\n{:?}", value)
                    .to_string(),
            },
        }
    }
}

impl<T: Debug> From<ibmcloud_iam_api::apis::Error<T>> for ServiceError {
    fn from(value: ibmcloud_iam_api::apis::Error<T>) -> Self {
        match value {
            ibmcloud_iam_api::apis::Error::ResponseError(e) => ServiceError {
                code: match e.status.as_u16() {
                    400 => ExitCode::IAMAPIBadRequest,
                    401 => ExitCode::IAMAPIUnauthenticated,
                    403 => ExitCode::IAMAPIForbidden,
                    404 => ExitCode::IAMAPINotFound,
                    409 => ExitCode::IAMAPIConflict,
                    _ => ExitCode::IAMAPIUnhandledError,
                },
                message: e.content,
            },
            _ => ServiceError {
                code: ExitCode::IAMAPIUnhandledError,
                message: format!("Unhandled IBM IAM API error:\n\n{:?}", value).to_string(),
            },
        }
    }
}


fn get_account_config(filename: Option<&str>, name: Option<&str>) -> Result<AccountEntry, ConfigError> {
    if let Ok(token) = std::env::var("QISKIT_IBM_TOKEN") {

        let instance = std::env::var("QISKIT_IBM_INSTANCE")
            .map_err(|_| ConfigError::MissingCredentials)?; 

        return Ok(AccountEntry {
            token,
            instance: Some(instance),
            channel: std::env::var("QISKIT_IBM_CHANNEL").unwrap_or_else(|_| "ibm_quantum".to_string()),
            url: std::env::var("QISKIT_IBM_URL").unwrap_or_else(|_| "https://auth.quantum-computing.ibm.com/api".to_string()),
            verify: true,
            private_endpoint: false,
            proxies: None,
        });
    }

    let filename = match filename {
        Some(path) => path.to_string(),
        None => {
            let home = std::env::var("HOME").map_err(|_| ConfigError::HomeDirectoryNotFound)?;
            format!("{}/.qiskit/qiskit-ibm.json", home)
        }
    };

    let file_path = Path::new(&filename);

    let file = File::open(file_path).map_err(|e| ConfigError::FileNotFound {
        path: filename.clone(),
        error: e.to_string(), 
    })?;

    let reader = BufReader::new(file);
    let accounts: HashMap<String, AccountEntry> = serde_json::from_reader(reader)
     .map_err(|e| ConfigError::InvalidJson {
        path: filename.clone(),
        error: e.to_string(),
    })?;

    match name {
        Some(name) => { accounts.get(name)
            .cloned()
            .ok_or_else(|| ConfigError::AccountNotFound {
                name: name.to_string(),
                available: accounts.keys().cloned().collect(),
            })
        }
        None => { accounts
            .get("default")
            .or_else(|| accounts.get("default-ibm-quantum-platform"))
            .or_else(|| accounts.get("default-ibm-cloud"))
            .cloned()
            .ok_or_else(|| ConfigError::NoDefaultAccount {
                available: accounts.keys().cloned().collect(),
            })
        }    
    }
}


#[derive(Debug)]
pub enum ConfigError {
    MissingCredentials,
    HomeDirectoryNotFound,
    FileNotFound { path: String, error: String },
    InvalidJson { path: String, error: String },
    AccountNotFound { name: String, available: Vec<String> },
    NoDefaultAccount { available: Vec<String> },
}


impl std::fmt::Display for ConfigError {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            ConfigError::HomeDirectoryNotFound => {
                write!(f, "[CONFIG ERROR] Could not determine home directory.\n\
                    \nTroubleshooting:\n\
                    - Ensure HOME environment variable is set\n\
                    - Or use QISKIT_IBM_TOKEN environment variable instead")
            }
            ConfigError::FileNotFound { path, error } => {
                write!(f, "[CONFIG ERROR] Configuration file not found\n\
                    \nFile: {}\n\
                    Error: {}\n\
                    \nSolutions:\n\
                    1. Set environment variables:\n\
                       export QISKIT_IBM_TOKEN=\"your_token\"\n\
                       export QISKIT_IBM_INSTANCE=\"your_instance\"\n\
                    2. Create config file at: ~/.qiskit/qiskit-ibm.json\n\
                       See: https://github.com/Qiskit/qiskit-ibm-runtime#configuration",
                    path, error)
            }
            ConfigError::InvalidJson { path, error } => {
                write!(f, "[CONFIG ERROR] Invalid JSON in configuration file\n\
                    \nFile: {}\n\
                    Parse error: {}\n\
                    \nPlease check your JSON syntax at: {}",
                    path, error, path)
            }
            ConfigError::AccountNotFound { name, available } => {
                write!(f, "[CONFIG ERROR] Account '{}' not found in configuration\n\
                    \nAvailable accounts: {:?}\n\
                    \nPlease use one of the available account names or add '{}' to your config file.",
                    name, available, name)
            }
            ConfigError::NoDefaultAccount { available } => {
                write!(f, "[CONFIG ERROR] No default account found in configuration\n\
                    \nAvailable accounts: {:?}\n\
                    \nExpected one of:\n\
                    - 'default'\n\
                    - 'default-ibm-quantum-platform'\n\
                    - 'default-ibm-cloud'\n\
                    \nPlease rename one of your accounts to a default name.",
                    available)
            }
            ConfigError::MissingCredentials => {
                write!(f, "[CONFIG ERROR] No credentials found\n\
                    \nPlease provide credentials using either:\n\
                    1. Environment variables:\n\
                       export QISKIT_IBM_TOKEN=\"your_token\"\n\
                       export QISKIT_IBM_INSTANCE=\"your_instance\"\n\
                    2. Configuration file at ~/.qiskit/qiskit-ibm.json")
            }
        }
    }
}


impl From<ConfigError> for ServiceError {
    fn from(value: ConfigError) -> Self {
        match value {
            ConfigError::HomeDirectoryNotFound => ServiceError {
                code: ExitCode::ConfigHomeDirectoryNotFound,
                message: "Could not determine home directory.\n\
                    \nTroubleshooting:\n\
                    - Ensure HOME environment variable is set\n\
                    - Or use QISKIT_IBM_TOKEN environment variable instead".to_string(),
            },
            ConfigError::FileNotFound { path, error } => ServiceError {
                code: ExitCode::ConfigFileNotFound,
                message: format!(
                    "Configuration file not found at '{}': {}\n\
                    \nPlease either:\n\
                    1. Set environment variables: QISKIT_IBM_TOKEN, QISKIT_IBM_INSTANCE\n\
                    2. Create config file at: ~/.qiskit/qiskit-ibm.json",
                    path, error
                ),
            },
            ConfigError::InvalidJson { path, error } => ServiceError {
                code: ExitCode::ConfigInvalidJson,
                message: format!(
                    "Invalid JSON in config file '{}':\n{}\n\
                    \nPlease check your JSON syntax.",
                    path, error
                ),
            },
            ConfigError::AccountNotFound { name, available } => ServiceError {
                code: ExitCode::ConfigAccountNotFound,
                message: format!(
                    "Account '{}' not found in configuration.\n\
                    \nAvailable accounts: {:?}\n\
                    \nPlease use one of the available account names.",
                    name, available
                ),
            },
            ConfigError::NoDefaultAccount { available } => ServiceError {
                code: ExitCode::ConfigNoDefaultAccount,
                message: format!(
                    "No default account found in configuration.\n\
                    \nAvailable accounts: {:?}\n\
                    \nExpected one of: 'default', 'default-ibm-quantum-platform', 'default-ibm-cloud'",
                    available
                ),
            },
            ConfigError::MissingCredentials => ServiceError {
                code: ExitCode::ConfigMissingCredentials,
                message: "No credentials found.\n\
                    \nPlease set QISKIT_IBM_TOKEN environment variable or create ~/.qiskit/qiskit-ibm.json".to_string(),
            },
        }
    }
}


#[derive(Clone, Debug)]
pub struct Service {
    account: Account,
    instances: Vec<Instance>,
    quantum_config: ibm_quantum_platform_api::apis::configuration::Configuration,
}

impl Service {
    pub fn new(account: Account, instances: Vec<Instance>) -> Self {
        let mut quantum_config =
            ibm_quantum_platform_api::apis::configuration::Configuration::default();
        quantum_config.user_agent = Some("qiskit-ibm-runtime-rs/0.0.1".to_string());
        quantum_config.api_key = Some(ibm_quantum_platform_api::apis::configuration::ApiKey {
            key: account.get_access_token().unwrap().to_string(),
            prefix: Some("Bearer".to_string()),
        });

        Service {
            account,
            instances,
            quantum_config,
        }
    }
}

#[derive(Clone, Debug)]
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

#[derive(Clone, Debug)]
pub struct Instance {
    pub crn: CString,
    pub name: CString,
}

pub async fn get_account_from_config(
    filename: Option<&str>,
    name: Option<&str>,
) -> Result<Account, ServiceError> {
    let config = get_account_config(filename, name)?;

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
    log_debug(&format!(
        "get_account_from_config response: {:?}",
        &response
    ));
    Ok(Account {
        config,
        token: response,
        iam_config,
    })
}

pub async fn list_instances(account: &Account) -> Result<Vec<Instance>, ServiceError> {
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
    .await?;
    log_debug(&format!("list_instances response: {:?}", &resp));
    let items = resp.items;
    Ok(items
        .into_iter()
        .filter_map(|x| {
            if let Some(doc) = x.doc {
                // Filter for only instances with backend allocations
                if doc.contains_key("extensions") {
                    return Some(Instance {
                        crn: CString::new(x.crn).unwrap(),
                        name: CString::new(x.name.unwrap()).unwrap(),
                    });
                }
            }
            None
        })
        .collect())
}

pub async fn get_backend(service: &Service, backend: &Backend) -> crate::qiskit_target::Target {
    let name = backend.response.name.clone();
    let crn = backend.instance.crn.to_str().unwrap();

    let backend_configuration = get_backend_configuration(
        &service.quantum_config,
        name.as_str(),
        crn,
        Some("2025-06-01"),
    )
    .await
    .unwrap();
    let backend_properties = get_backend_properties(
        &service.quantum_config,
        name.as_str(),
        crn,
        Some("2025-06-01"),
        None,
    )
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
        } else if gate == "rzz" {
            // TODO: Add RZZ support when we have angle wrapping in
            // Qiskit's target and C transpiler.
            //            crate::qiskit_target::ISAGate::RZZ
            log_warn(&concat!(
                "rzz gate's constraints cannot be represented in the target from C ",
                "currently. It is being excluded from the target generated for ",
                "this backend."
            ));
            continue;
        } else if gate == "reset" {
            target.add_reset(
                props
                    .into_iter()
                    .map(|(qubits, dur, err)| (qubits[0], [dur, err])),
            );
            continue;
        } else {
            log_warn(&format!(
                "{gate} can not be represented in the Target for {name} because it does not correspond to a Qiskit standard operation. It will not be included in the target generated for this backend."));
            continue;
        };
        target.add_gate(
            gate,
            props
                .into_iter()
                .map(|(qubits, dur, err)| (qubits, [dur, err])),
        );
    }
    let measure_props = backend_properties["qubits"]
        .as_array()
        .unwrap()
        .iter()
        .enumerate()
        .map(|(idx, props)| {
            let mut error = None;
            let mut duration = None;
            for prop in props.as_array().unwrap() {
                let prop = prop.as_object().unwrap();
                let name = prop["name"].as_str().unwrap();
                if name == "readout_error" {
                    error = prop["value"].as_f64();
                } else if name == "readout_length" {
                    duration = prop["value"].as_f64();
                }
            }
            (idx as u32, [duration, error])
        });
    target.add_measure(measure_props);
    target
}

pub async fn submit_estimator_job(
    service: &Service,
    backend: &Backend,
    circuit: &crate::qiskit_circuit::Circuit,
    observable: &crate::qiskit_observable::SparseObservable,
    runtime: Option<String>,
    tags: Option<Vec<String>>,
) -> Result<Job, ServiceError> {
    let crn = backend.instance.crn.to_str().unwrap();
    let job_payload = crate::generate_job_params::create_estimator_job_payload(
        circuit,
        observable,
        backend.response.name.clone(),
        None,
        runtime,
        tags,
    );
    let res = create_job(
        &service.quantum_config,
        crn,
        Some("2025-06-01"),
        None,
        Some(CreateJobRequest::CreateJobRequestOneOf(Box::new(
            job_payload,
        ))),
    )
    .await?;
    log_debug(&format!("submit_sampler_job response: {:?}", res));
    Ok(Job {
        instance: backend.instance.clone(),
        response: res,
    })
}

pub async fn submit_sampler_job(
    service: &Service,
    backend: &Backend,
    circuit: &crate::qiskit_circuit::Circuit,
    shots: Option<i32>,
    runtime: Option<String>,
    tags: Option<Vec<String>>,
) -> Result<Job, ServiceError> {
    let crn = backend.instance.crn.to_str().unwrap();
    let job_payload = crate::generate_job_params::create_sampler_job_payload(
        circuit,
        backend.response.name.clone(),
        shots,
        runtime,
        tags,
    );
    let res = create_job(
        &service.quantum_config,
        crn,
        Some("2025-06-01"),
        None,
        Some(CreateJobRequest::CreateJobRequestOneOf(Box::new(
            job_payload,
        ))),
    )
    .await?;
    log_debug(&format!("submit_sampler_job response: {:?}", res));
    Ok(Job {
        instance: backend.instance.clone(),
        response: res,
    })
}

pub async fn get_job_details(service: &Service, job: &Job) -> Result<JobDetails, ServiceError> {
    let crn = job.instance.crn.to_str().unwrap();
    let details = get_job_details_jid(
        &service.quantum_config,
        crn,
        &job.response.id,
        Some("2025-06-01"),
        None,
    )
    .await?;
    log_debug(&format!("get_job_details response: {:?}", details));
    Ok(JobDetails(details))
}

#[derive(Debug)]
pub struct Samples(pub Vec<String>);

pub async fn get_sampler_job_results(
    service: &Service,
    job: &Job,
) -> Result<Samples, ServiceError> {
    let crn = job.instance.crn.to_str().unwrap();
    let details = get_sampler_job_results_jid(
        &service.quantum_config,
        crn,
        &job.response.id,
        Some("2025-06-01"),
    )
    .await?;
    log_debug(&format!("get_job_result response: {:?}", details));
    let res = Ok(Samples(
        details
            .results
            .iter()
            .flat_map(|x| x.data["meas"].samples.iter())
            .cloned()
            .collect(),
    ));
    res
}

#[derive(Debug)]
pub struct ExpectationValues(pub Vec<f64>);

pub async fn get_estimator_job_results(
    service: &Service,
    job: &Job,
) -> Result<ExpectationValues, ServiceError> {
    let crn = job.instance.crn.to_str().unwrap();
    let details = get_estimator_job_results_jid(
        &service.quantum_config,
        crn,
        &job.response.id,
        Some("2025-06-01"),
    )
    .await?;
    log_debug(&format!("get_job_result response: {:?}", details));
    Ok(ExpectationValues(
        details.results.iter().map(|x| x.data["evs"]).collect(),
    ))
}

pub async fn get_job_status(service: &Service, job: &Job) -> Result<JobStatus, ServiceError> {
    let details = get_job_details(service, job).await?;
    Ok(details.status())
}

pub async fn get_backends(service: &Service) -> Result<BackendSearchResults, ServiceError> {
    let mut backends = Vec::new();
    let mut ptrs = Vec::new();
    for instance in &service.instances {
        let crn = instance.crn.to_str().unwrap();
        let Ok(resp) = list_backends(&service.quantum_config, Some("2025-06-01"), crn).await else {
            let instance_name = instance.name.to_str().unwrap();
            log_warn(&format!(
                "Failed to list backends for instance: {} ({})",
                instance_name, crn
            ));
            continue;
        };
        log_debug(&format!("get_backends response: {:?}", &resp));
        for backend in resp.devices.into_iter().flatten() {
            let backend = Box::new(Backend {
                name: CString::new(backend.name.as_str()).unwrap(),
                instance: instance.clone(),
                response: backend,
            });
            ptrs.push(backend.as_ref() as *const Backend);
            backends.push(backend);
        }
    }
    Ok(BackendSearchResults { backends, ptrs })
}
