use serde::{Deserialize, Serialize};
use std::fs::File;
use std::io::BufReader;
use std::path::Path;

use ibm_quantum_platform_api::apis::backends_api::{
    get_backend_configuration, get_backend_properties, list_backends,
};
use ibm_quantum_platform_api::apis::jobs_api::create_job;
use ibm_quantum_platform_api::models::CreateJobRequest;
use ibmcloud_global_search_api::apis::configuration::Configuration as SearchConfiguration;
use ibmcloud_global_search_api::apis::search_api::search;
use ibmcloud_iam_api::apis::configuration::Configuration;
use ibmcloud_iam_api::apis::token_operations_api::get_token_api_key;
use ibmcloud_iam_api::models::token_response::TokenResponse;

use std::collections::HashMap;

#[derive(Deserialize, Serialize, Clone)]
pub struct AccountEntry {
    channel: String,
    #[serde(default)]
    private_endpoint: bool,
    token: String,
    url: String,
    proxies: Option<ProxyConfiguration>,
    #[serde(default)]
    verify: bool,
}

#[derive(Deserialize, Serialize, Clone)]
struct ProxyConfiguration {
    urls: Option<HashMap<String, String>>,
    username_ntlm: Option<String>,
    password_ntlm: Option<String>,
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
    config: AccountEntry,
    token: TokenResponse,
    iam_config: Configuration,
}

impl Account {
    pub fn get_access_token(&self) -> Option<&str> {
        self.token.access_token.as_deref()
    }
}

pub async fn get_account_from_config(filename: Option<&str>, name: Option<&str>) -> Account {
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
    .await
    .unwrap();
    Account {
        config,
        token: response,
        iam_config,
    }
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
    backend: String,
    circuit: &crate::qiskit_circuit::Circuit,
    shots: Option<i32>,
    runtime: Option<String>,
    tags: Option<Vec<String>>,
) {
    let mut config = ibm_quantum_platform_api::apis::configuration::Configuration::default();
    config.user_agent = Some("qiskit-ibm-runtime-rs/0.0.1".to_string());
    config.api_key = Some(ibm_quantum_platform_api::apis::configuration::ApiKey {
        key: account.get_access_token().unwrap().to_string(),
        prefix: Some("Bearer".to_string()),
    });
    let crns = list_instances(&account).await;
    let crn = crns[0].as_str();
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
    .await
    .unwrap();
    println!("result: {:?}", res);
}

pub async fn get_backends(account: Account) -> Vec<String> {
    let mut config = ibm_quantum_platform_api::apis::configuration::Configuration::default();
    config.user_agent = Some("qiskit-ibm-runtime-rs/0.0.1".to_string());
    config.api_key = Some(ibm_quantum_platform_api::apis::configuration::ApiKey {
        key: account.get_access_token().unwrap().to_string(),
        prefix: Some("Bearer".to_string()),
    });
    let crns = list_instances(&account).await;
    let crn = crns[0].as_str();
    let resp = list_backends(&config, Some("2025-06-01"), crn).await;
    resp.unwrap()
        .devices
        .unwrap()
        .into_iter()
        .map(|x| x.name)
        .collect()
}
