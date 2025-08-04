use std::ffi::{c_char, CStr};
use std::fs::File;
use std::io::prelude::*;
use std::path::Path;
use crate::ExitCode;
use crate::generate_job_params::create_sampler_job_payload;
use crate::generate_qpy::generate_qpy_payload;
use crate::pointers::const_ptr_as_ref;
use crate::qiskit_circuit::Circuit;
use crate::qiskit_ffi::QkCircuit;

use crate::service::{get_account_from_config, get_backends, get_job_details, get_job_status, submit_sampler_job, Backend, BackendSearchResults, Job, JobDetails, Service, ServiceError};


fn log_err(e: &ServiceError) {
    if !std::env::var("QISKIT_IBM_RUNTIME_LOG_LEVEL").is_err() {
        eprintln!("** ERROR: {:?}", e)
    }
}

macro_rules! check_result {
    ($expr:expr) => {
        match $expr {
            Ok(val) => val,
            Err(e) => {
                log_err(&e);
                return e.code();
            },
        }
    };
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
pub unsafe extern "C" fn qkrt_service_new(out: *mut *mut Service) -> ExitCode {
    if out.is_null() {
        return ExitCode::NullPointerError;
    }
    *out = std::ptr::null_mut();
    let rt = tokio::runtime::Builder::new_current_thread()
        .enable_all()
        .build()
        .unwrap();
    let account = check_result!(rt.block_on(get_account_from_config(None, None)));
    *out = Box::into_raw(Box::new(Service::new(account)));
    ExitCode::Success
}

#[no_mangle]
pub unsafe extern "C" fn qkrt_service_free(service: *mut Service) {
    if !service.is_null() {
        unsafe {
            drop(Box::from_raw(service));
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn qkrt_backend_search(out: *mut *mut BackendSearchResults, service: *const Service) -> ExitCode {
    if out.is_null() {
        return ExitCode::NullPointerError;
    }
    *out = std::ptr::null_mut();
    let rt = tokio::runtime::Builder::new_current_thread()
        .enable_all()
        .build()
        .unwrap();
    let account = check_result!(rt.block_on(get_backends(const_ptr_as_ref(service))));
    *out = Box::into_raw(Box::new(account));
    ExitCode::Success
}

#[no_mangle]
pub unsafe extern "C" fn qkrt_backend_search_results_free(results: *mut BackendSearchResults) {
    if !results.is_null() {
        unsafe {
            drop(Box::from_raw(results));
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn qkrt_backend_search_results_length(results: *const BackendSearchResults) -> u64 {
    let results = const_ptr_as_ref(results);
    results.len() as u64
}

#[no_mangle]
pub unsafe extern "C" fn qkrt_backend_search_results_data(results: *const BackendSearchResults) -> *const *const Backend {
    let results = const_ptr_as_ref(results);
    results.data_ptr()
}

#[no_mangle]
pub unsafe extern "C" fn qkrt_backend_name(backend: *const Backend) -> *const c_char {
    let backend = const_ptr_as_ref(backend);
    backend.name.as_ptr()
}

#[no_mangle]
pub unsafe extern "C" fn qkrt_sampler_job_run(
    out: *mut *mut Job,
    service: *const Service,
    backend: *const Backend,
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

    let service = const_ptr_as_ref(service);
    let backend = const_ptr_as_ref(backend);
    let runtime = if runtime.is_null() {
        None
    } else {
        unsafe { Some(CStr::from_ptr(runtime).to_str().unwrap().to_string()) }
    };
    let shots = if shots < 0 { None } else { Some(shots) };
    let job = check_result!(rt.block_on(submit_sampler_job(
        &service,
        backend.name().to_string(),
        &Circuit(circuit),
        shots,
        runtime,
        None,
    )));
    *out = Box::into_raw(Box::new(job));
    ExitCode::Success
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
pub unsafe extern "C" fn qkrt_job_details(out: *mut *mut JobDetails, service: *const Service, job: *const Job) -> ExitCode {
    if out.is_null() {
        return ExitCode::NullPointerError;
    }
    *out = std::ptr::null_mut();
    let rt = tokio::runtime::Builder::new_current_thread()
        .enable_all()
        .build()
        .unwrap();
    let service = const_ptr_as_ref(service);
    let job = const_ptr_as_ref(job);
    let details = check_result!(rt.block_on(get_job_details(
        service,
        job,
    )));
    *out = Box::into_raw(Box::new(details));
    ExitCode::Success
}

#[no_mangle]
pub unsafe extern "C" fn qkrt_job_details_free(details: *mut JobDetails) {
    if !details.is_null() {
        unsafe {
            drop(Box::from_raw(details));
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn qkrt_job_status(out: *mut u32, service: *const Service, job: *const Job) -> ExitCode {
    let rt = tokio::runtime::Builder::new_current_thread()
        .enable_all()
        .build()
        .unwrap();
    let service = const_ptr_as_ref(service);
    let job = const_ptr_as_ref(job);
    let status = check_result!(rt.block_on(get_job_status(
        service,
        job,
    )));
    *out = status as u32;
    ExitCode::Success
}

#[no_mangle]
pub extern "C" fn get_access_token() {
    let rt = tokio::runtime::Builder::new_current_thread()
        .enable_all()
        .build()
        .unwrap();

    let account = rt.block_on(get_account_from_config(None, None)).unwrap();
    println!("run");
    println!("token: {:?}", account.get_access_token());
}
