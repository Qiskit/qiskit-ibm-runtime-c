use std::ffi::{c_char, CStr};
use std::fs::File;
use std::io::prelude::*;
use std::path::Path;

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
