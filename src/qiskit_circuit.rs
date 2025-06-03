use crate::qiskit_ffi;
use std::ffi::CStr;

pub struct CircuitInstruction<'a> {
    pub name: String,
    pub qubits: &'a [u32],
    pub clbits: &'a [u32],
    pub params: &'a [f64],
}

pub struct CircuitInstructions<'a> {
    len: usize,
    ptr: *mut qiskit_ffi::QkCircuitInstruction,
    index: usize,
    circuit: &'a Circuit,
}

impl<'a> Drop for CircuitInstructions<'a> {
    fn drop(&mut self) {
        unsafe {
            std::alloc::dealloc(
                self.ptr as *mut u8,
                std::alloc::Layout::new::<qiskit_ffi::QkCircuitInstruction>(),
            );
        }
    }
}

impl<'a> ExactSizeIterator for CircuitInstructions<'a> {
    fn len(&self) -> usize {
        self.len - self.index
    }
}

impl<'a> Iterator for CircuitInstructions<'a> {
    type Item = CircuitInstruction<'a>;

    fn next(&mut self) -> Option<Self::Item> {
        if self.index >= self.len {
            return None;
        }
        let out = unsafe {
            qiskit_ffi::qk_circuit_get_instruction(self.circuit.0, self.index, self.ptr);
            let raw_inst = self.ptr.as_ref().unwrap();
            let qubits = std::slice::from_raw_parts(raw_inst.qubits, raw_inst.num_qubits as usize);
            let clbits = std::slice::from_raw_parts(raw_inst.clbits, raw_inst.num_clbits as usize);
            let params = std::slice::from_raw_parts(raw_inst.params, raw_inst.num_params as usize);
            let c_name: Box<CStr> = Box::from(CStr::from_ptr(raw_inst.name));
            let name = c_name.into_c_string().into_string().unwrap();
            let out = Some(CircuitInstruction {
                name,
                qubits,
                clbits,
                params,
            });
            qiskit_ffi::qk_circuit_instruction_clear(self.ptr);
            out
        };
        self.index += 1;
        out
    }
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
        CircuitInstructions {
            len: num_inst,
            circuit: self,
            ptr: unsafe {
                std::alloc::alloc(std::alloc::Layout::new::<qiskit_ffi::QkCircuitInstruction>())
                    as *mut qiskit_ffi::QkCircuitInstruction
            },
            index: 0,
        }
    }
}
