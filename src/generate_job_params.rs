use crate::generate_qpy;
use crate::qiskit_circuit;
use flate2::write::ZlibEncoder;
use flate2::Compression;
use std::io::prelude::*;

pub fn generate_single_pubs_payload(
    circuit: &qiskit_circuit::Circuit,
) -> Vec<ibm_quantum_platform_api::models::SamplerV2InputPubsInner> {
    let base64 = base64_simd::STANDARD;
    let qpy = generate_qpy::generate_qpy_payload(circuit).unwrap();
    let mut compress = ZlibEncoder::new(Vec::new(), Compression::default());
    compress.write_all(&qpy).unwrap();
    let compressed_qpy = compress.finish().unwrap();
    let encoded_circuit = base64.encode_to_string(compressed_qpy);
    vec![ibm_quantum_platform_api::models::SamplerV2InputPubsInner::new(encoded_circuit)]
}

pub fn create_sampler_job_payload(
    circuit: &qiskit_circuit::Circuit,
    backend: String,
    shots: Option<i32>,
    runtime: Option<String>,
    tags: Option<Vec<String>>,
) -> ibm_quantum_platform_api::models::CreateJobRequestOneOf {
    let pubs = generate_single_pubs_payload(circuit);
    let sampler_input = ibm_quantum_platform_api::models::SamplerV2Input {
        pubs,
        options: None,
        shots: Some(shots),
        support_qiskit: None,
        version: 2,
    };
    let params = Box::new(
        ibm_quantum_platform_api::models::CreateJobRequestOneOfAllOfParams::SamplerV2Input(
            Box::new(sampler_input),
        ),
    );
    ibm_quantum_platform_api::models::CreateJobRequestOneOf {
        program_id: "sampler".to_string(),
        backend,
        runtime,
        tags,
        log_level: None,
        cost: None,
        session_id: None,
        params: Some(params),
        private: None,
        version: 2,
    }
}
