use std::ffi::{c_char, CStr};
use std::fs::File;
use std::io::prelude::*;
use std::path::Path;

use crate::generate_job_params::create_sampler_job_payload;
use crate::generate_qpy::generate_qpy_payload;
use crate::qiskit_circuit::Circuit;
use crate::qiskit_ffi::QkCircuit;

use crate::service::{get_account_from_config, get_backends, submit_sampler_job, ExitCode, Job, ServiceError};


fn log_err(e: &ServiceError) {
    if !std::env::var("QISKIT_IBM_RUNTIME_LOG_LEVEL").is_err() {
        eprintln!("{:?}", e)
    }
}


/// This function only generates ISA static circuit QPY with no parameters
///
/// # Safety
///
/// It's C so it's never safe
#[no_mangle]
pub unsafe extern "C" fn generate_qpy(circuit: *mut QkCircuit, filename: *const c_char) {
    let circuit = Circuit(circuit);
    let path = unsafe { Path::new(CStr::from_ptr(filename).to_str().unwrap()) };
    let mut file = File::create(path).unwrap();
    let qpy = generate_qpy_payload(&circuit).unwrap();
    file.write_all(&qpy).unwrap();
}

/// Save a sampler job payload JSON body to a file
///
/// # Safety
///
/// It's C so it's never safe
#[no_mangle]
pub unsafe extern "C" fn qkrt_sampler_job_write_payload(
    circuit: *mut QkCircuit,
    shots: i32,
    backend: *const c_char,
    runtime: *const c_char,
    filename: *const c_char,
) {
    let circuit = Circuit(circuit);
    let path = unsafe { Path::new(CStr::from_ptr(filename).to_str().unwrap()) };
    let file = File::create(path).unwrap();
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

#[no_mangle]
pub unsafe extern "C" fn qkrt_sampler_job_run(
    out: *mut *mut Job,
    circuit: *mut QkCircuit,
    shots: i32,
    runtime: *const c_char,
) -> ExitCode {
    if out.is_null() {
        return ExitCode::NullPointerError;
    }
    *out = std::ptr::null_mut();
    let rt = tokio::runtime::Builder::new_current_thread()
        .enable_all()
        .build()
        .unwrap();
    let account = rt.block_on(get_account_from_config(None, None));
    let backends = rt.block_on(get_backends(account.clone()));
    let backend = backends.last().unwrap().clone();
    let runtime = if runtime.is_null() {
        None
    } else {
        unsafe { Some(CStr::from_ptr(runtime).to_str().unwrap().to_string()) }
    };
    let shots = if shots < 0 { None } else { Some(shots) };
    let res = rt.block_on(submit_sampler_job(
        account,
        backend,
        &Circuit(circuit),
        shots,
        runtime,
        None,
    ));
    match res {
        Ok(job) => {
            *out = Box::into_raw(Box::new(job));
            ExitCode::Success
        },
        Err(e) => {
            log_err(&e);
            e.code()
        },
    }
}

#[no_mangle]
pub unsafe extern "C" fn qkrt_job_free(job: *mut Job) {
    if !job.is_null() {
        unsafe {
            drop(Box::from_raw(job));
        }
    }
}

#[no_mangle]
pub extern "C" fn get_access_token() {
    let rt = tokio::runtime::Builder::new_current_thread()
        .enable_all()
        .build()
        .unwrap();

    let account = rt.block_on(get_account_from_config(None, None));
    println!("run");
    println!("token: {:?}", account.get_access_token());
}

#[no_mangle]
pub extern "C" fn get_backend_names() {
    let rt = tokio::runtime::Builder::new_current_thread()
        .enable_all()
        .build()
        .unwrap();

    let account = rt.block_on(get_account_from_config(None, None));
    let backends = rt.block_on(get_backends(account));
    println!("backends: {:?}", backends);
}
