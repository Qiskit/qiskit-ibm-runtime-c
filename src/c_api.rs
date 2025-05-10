use std::ffi::{c_char, CStr};
use std::fs::File;
use std::io::prelude::*;
use std::path::Path;

use serde_json;

use crate::generate_job_params::create_sampler_job_payload;
use crate::generate_qpy::generate_qpy_payload;
use crate::qiskit_circuit::Circuit;
use crate::qiskit_ffi::QkCircuit;

/// This function only generates ISA static circuit QPY with no parameters
#[no_mangle]
pub extern "C" fn generate_qpy(circuit: *mut QkCircuit, filename: *const c_char) {
    let circuit = Circuit(circuit);
    let path = unsafe { Path::new(CStr::from_ptr(filename).to_str().unwrap()) };
    let mut file = File::create(&path).unwrap();
    let qpy = generate_qpy_payload(&circuit).unwrap();
    file.write_all(&qpy).unwrap();
}

/// Save a sampler job payload JSON body to a file
#[no_mangle]
pub extern "C" fn save_sampler_job_payload(
    circuit: *mut QkCircuit,
    shots: i32,
    backend: *const c_char,
    runtime: *const c_char,
    filename: *const c_char,
) {
    let circuit = Circuit(circuit);
    let path = unsafe { Path::new(CStr::from_ptr(filename).to_str().unwrap()) };
    let file = File::create(&path).unwrap();
    let backend = unsafe { CStr::from_ptr(backend).to_str().unwrap().to_string() };
    let runtime = if runtime.is_null() {
        None
    } else {
        unsafe { Some(CStr::from_ptr(runtime).to_str().unwrap().to_string()) }
    };
    //    let tags = if tags.is_null() {
    //        None
    //    } else {
    //        unsafe { Some(CStr::from_ptr(tags).to_str().unwrap().to_string()) }
    //    };

    let model = create_sampler_job_payload(&circuit, backend, Some(shots), runtime, None);
    serde_json::to_writer_pretty(file, &model).unwrap();
}
