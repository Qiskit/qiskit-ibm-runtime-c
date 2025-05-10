use std::io::prelude::*;
use std::collections::HashMap;
use crate::generate_qpy;
use crate::qiskit_circuit;
use flate2::Compression;
use flate2::write::ZlibEncoder;
use serde_json;
use ibm_quantum_platform_api;


pub fn generate_single_pubs_payload(circuit: &qiskit_circuit::Circuit) -> HashMap<String, serde_json::Value> {
    let base64 = base64_simd::STANDARD;
    let qpy = generate_qpy::generate_qpy_payload(circuit).unwrap();
    let mut compress = ZlibEncoder::new(Vec::new(), Compression::default());
    compress.write_all(&qpy).unwrap();
    let compressed_qpy = compress.finish().unwrap();
    let encoded_circuit = base64.encode_to_string(compressed_qpy);

    let params_obj = serde_json::json!([
            {
                "__type__": "QuantumCircuit",
                "__value__": encoded_circuit,
            },
            null
    ]);
    let mut out = HashMap::with_capacity(1);
    out.insert("pubs".to_string(), params_obj);
    out.insert("version".to_string(), serde_json::json!("2"));
    out.insert("support_qiskit".to_string(), serde_json::json!(true));
    out
}

pub fn create_sampler_job_payload(circuit: &qiskit_circuit::Circuit, backend: String, runtime: Option<String>, tags: Option<Vec<String>>) -> ibm_quantum_platform_api::models::CreateJobRequestOneOf {

    let params = generate_single_pubs_payload(circuit);
    ibm_quantum_platform_api::models::CreateJobRequestOneOf {
        program_id: "sampler".to_string(),
        backend,
        runtime,
        tags,
        log_level: None,
        cost: None,
        session_id: None,
        params: Some(params),
    }
}
