use crate::qiskit_ffi;
use std::ffi::CStr;

pub struct CircuitInstruction<'a> {
    pub name: String,
    pub qubits: &'a [u32],
    pub clbits: &'a [u32],
    pub params: &'a [f64],
}

pub struct Circuit(pub *mut qiskit_ffi::QkCircuit);

impl Circuit {
    pub fn num_qubits(&self) -> u32 {
        unsafe { qiskit_ffi::qk_circuit_num_qubits(self.0) }
    }

    pub fn num_clbits(&self) -> u32 {
        unsafe { qiskit_ffi::qk_circuit_num_clbits(self.0) }
    }

    pub fn num_instructions(&self) -> usize {
        unsafe { qiskit_ffi::qk_circuit_num_instructions(self.0) }
    }

    pub fn get_circuit_instructions(
        &self,
    ) -> impl ExactSizeIterator<Item = CircuitInstruction<'_>> + '_ {
        let num_inst = self.num_instructions();
        (0..num_inst).map(|index| unsafe {
            let raw_inst = qiskit_ffi::qk_circuit_get_instruction(self.0, index);
            let qubits = std::slice::from_raw_parts(raw_inst.qubits, raw_inst.num_qubits as usize);
            let clbits = std::slice::from_raw_parts(raw_inst.clbits, raw_inst.num_clbits as usize);
            let params = std::slice::from_raw_parts(raw_inst.params, raw_inst.num_params as usize);
            let c_name: Box<CStr> = Box::from(CStr::from_ptr(raw_inst.name));
            let name = c_name.into_c_string().into_string().unwrap();
            CircuitInstruction {
                name,
                qubits,
                clbits,
                params,
            }
        })
    }
}
