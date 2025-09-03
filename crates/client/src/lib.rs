mod c_api;
mod generate_job_params;
pub mod generate_qpy;
mod pointers;
pub mod qiskit_circuit;
mod qiskit_ffi;
pub mod qiskit_target;
mod qpy_formats;
mod service;

pub use c_api::generate_qpy;

// TODO: should we make these errors specific to the internal service error codes rather than
//       just generic HTTP response codes? The internal codes for IBM quantum are at least
//       present in the API response error message, but perhaps they're available in a field
//       we can match on.
#[derive(Copy, Clone, Debug)]
#[repr(u32)]
pub enum ExitCode {
    /// Success.
    Success = 0,
    /// Unexpected null pointer.
    NullPointerError = 1,
    /// Pointer is not aligned to expected data.
    AlignmentError = 2,
    /// An invalid argument was provided during the function call.
    BadArgumentError = 3,

    /// An error we didn't anticipate from IBM Quantum platform.
    QuantumAPIUnhandledError = 100,
    /// The IBM Quantum platform was called with invalid parameters.
    QuantumAPIBadRequest = 101,
    /// The IBM Quantum platform needs to be authenticated before this action.
    QuantumAPIUnauthenticated = 102,
    /// The IBM Quantum platform user is not permitted to perform this action.
    QuantumAPIForbidden = 103,
    /// The IBM Quantum platform returned a 404.
    QuantumAPINotFound = 104,
    /// The IBM Quantum platform returned a 409.
    QuantumAPIConflict = 105,

    /// An error we didn't anticipate from IBM Global Search.
    GlobalSearchAPIUnhandledError = 200,
    /// The IBM Global Search was called with invalid parameters.
    GlobalSearchAPIBadRequest = 201,
    /// The IBM Global Search needs to be authenticated before this action.
    GlobalSearchAPIUnauthenticated = 202,
    /// The IBM Global Search user is not permitted to perform this action.
    GlobalSearchAPIForbidden = 203,
    /// The IBM Global Search returned a 404.
    GlobalSearchAPINotFound = 204,
    /// The IBM Global Search returned a 409.
    GlobalSearchAPIConflict = 205,

    /// An error we didn't anticipate from IBM IAM.
    IAMAPIUnhandledError = 300,
    /// The IBM IAM was called with invalid parameters.
    IAMAPIBadRequest = 301,
    /// The IBM IAM needs to be authenticated before this action.
    IAMAPIUnauthenticated = 302,
    /// The IBM IAM user is not permitted to perform this action.
    IAMAPIForbidden = 303,
    /// The IBM IAM returned a 404.
    IAMAPINotFound = 304,
    /// The IBM IAM returned a 409.
    IAMAPIConflict = 305,
}

fn log_err(e: &impl AsRef<str>) {
    match std::env::var("QISKIT_IBM_RUNTIME_LOG_LEVEL") {
        Ok(level)
            if matches!(
                level.to_uppercase().as_str(),
                "ERROR" | "WARNING" | "INFO" | "DEBUG"
            ) =>
        {
            eprintln!("** ERROR: {}", e.as_ref())
        }
        _ => (),
    }
}

fn log_warn(e: &impl AsRef<str>) {
    match std::env::var("QISKIT_IBM_RUNTIME_LOG_LEVEL") {
        Ok(level) if matches!(level.to_uppercase().as_str(), "WARNING" | "INFO" | "DEBUG") => {
            eprintln!("* WARNING: {}", e.as_ref())
        }
        _ => (),
    }
}

fn log_debug(e: &impl AsRef<str>) {
    match std::env::var("QISKIT_IBM_RUNTIME_LOG_LEVEL") {
        Ok(level) if matches!(level.to_uppercase().as_str(), "DEBUG") => {
            eprintln!("[DEBUG]: {}", e.as_ref())
        }
        _ => (),
    }
}
