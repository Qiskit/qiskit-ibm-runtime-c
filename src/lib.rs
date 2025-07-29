mod c_api;
mod generate_job_params;
pub mod generate_qpy;
pub mod qiskit_circuit;
mod qiskit_ffi;
pub mod qiskit_target;
mod qpy_formats;
mod service;
mod pointers;

pub use c_api::generate_qpy;

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
    IBMQuantumAPIUnhandledError = 100,
    /// The IBM Quantum platform was called with invalid parameters.
    IBMQuantumAPIBadRequest = 101,
    /// The IBM Quantum platform needs to be authenticated before this action.
    IBMQuantumAPIUnauthenticated = 102,
    /// The IBM Quantum platform user is not permitted to perform this action.
    IBMQuantumAPIForbidden = 103,
    /// The IBM Quantum platform returned a 404.
    IBMQuantumAPINotFound = 104,
    /// The IBM Quantum platform returned a 409.
    IBMQuantumAPIConflict = 105,
}
