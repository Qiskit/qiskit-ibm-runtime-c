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

use crate::generate_qpy;
use crate::qiskit_circuit;
use crate::qiskit_observable;
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

pub fn generate_single_estimator_pubs_payload(
    circuit: &qiskit_circuit::Circuit,
    observable: &qiskit_observable::SparseObservable,
) -> Vec<ibm_quantum_platform_api::models::EstimatorV2InputPubsInner> {
    let base64 = base64_simd::STANDARD;
    let qpy = generate_qpy::generate_qpy_payload(circuit).unwrap();
    let mut compress = ZlibEncoder::new(Vec::new(), Compression::default());
    compress.write_all(&qpy).unwrap();
    let compressed_qpy = compress.finish().unwrap();
    let encoded_circuit = base64.encode_to_string(compressed_qpy);
    vec![
        ibm_quantum_platform_api::models::EstimatorV2InputPubsInner::Circuit(ibm_quantum_platform_api::models::estimator_v2_input_pubs_inner::EstimatorV2InputCircuit::new(encoded_circuit)),
        ibm_quantum_platform_api::models::EstimatorV2InputPubsInner::Observable(observable.to_term_map()),
    ]

}

static RESILIENCE_LEVEL_MAP: [ibm_quantum_platform_api::models::estimator_v2_input::ResilienceLevel; 3] = [
    ibm_quantum_platform_api::models::estimator_v2_input::ResilienceLevel::Variant0,
    ibm_quantum_platform_api::models::estimator_v2_input::ResilienceLevel::Variant1,
    ibm_quantum_platform_api::models::estimator_v2_input::ResilienceLevel::Variant2,
];


pub fn create_estimator_job_payload(
    circuit: &qiskit_circuit::Circuit,
    observable: &qiskit_observable::SparseObservable,
    backend: String,
    resilience_level: Option<u8>,
    runtime: Option<String>,
    tags: Option<Vec<String>>,
) -> ibm_quantum_platform_api::models::CreateJobRequestOneOf {
    let pubs = generate_single_estimator_pubs_payload(circuit, observable);
    let resilience_level = resilience_level.map(|x| {
        if  x > 2 {
            panic!("Invalid resilience_level: {}", x);
        } else {
            RESILIENCE_LEVEL_MAP[x as usize]
        }
    });
    let estimator_input = ibm_quantum_platform_api::models::EstimatorV2Input {
        pubs: vec![pubs],
        resilience_level,
        precision: None,
        options: None,
        support_qiskit: None,
        version: Some(ibm_quantum_platform_api::models::estimator_v2_input::Version::Variant2),
    };
    let params = Box::new(
        ibm_quantum_platform_api::models::CreateJobRequestOneOfAllOfParams::EstimatorV2Input(
            Box::new(estimator_input),
        ),
    );
    let tmp = ibm_quantum_platform_api::models::CreateJobRequestOneOf {
        program_id: "estimator".to_string(),
        backend,
        runtime,
        tags,
        log_level: None,
        cost: None,
        session_id: None,
        params: Some(params),
        private: None,
        version: 2,
    };
    println!("output: {:?}", serde_json::to_string(&tmp).unwrap());
    tmp
}
