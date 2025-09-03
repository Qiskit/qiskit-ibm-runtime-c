use crate::qiskit_ffi;

#[repr(u8)]
pub enum ISAGate {
    X = 3,
    SX = 13,
    CZ = 24,
    ECR = 26,
    RZ = 10,
    RX = 8,
    I = 2,
    CX = 22,
    RZZ = 41,
}

pub struct Target(pub(crate) *mut qiskit_ffi::QkTarget);

impl Drop for Target {
    fn drop(&mut self) {
        unsafe {
            qiskit_ffi::qk_target_free(self.0);
        }
    }
}

impl Target {
    pub fn new(num_qubits: u32) -> Self {
        Target(unsafe { qiskit_ffi::qk_target_new(num_qubits) })
    }

    pub fn set_dt(&mut self, dt: f64) {
        unsafe {
            qiskit_ffi::qk_target_set_dt(self.0, dt);
        }
    }

    pub fn add_gate(
        &mut self,
        gate: ISAGate,
        error_duration_map: impl Iterator<Item = (Vec<u32>, [Option<f64>; 2])>,
    ) {
        unsafe {
            let entry = qiskit_ffi::qk_target_entry_new(gate as u8);
            for (qargs, [duration, error]) in error_duration_map {
                qiskit_ffi::qk_target_entry_add_property(
                    entry,
                    qargs.clone().as_mut_ptr(),
                    qargs.len() as u32,
                    duration.unwrap_or(f64::NAN),
                    error.unwrap_or(f64::NAN),
                );
            }
            qiskit_ffi::qk_target_add_instruction(self.0, entry);
        }
    }

    pub fn add_measure(
        &mut self,
        error_duration_map: impl Iterator<Item = (u32, [Option<f64>; 2])>,
    ) {
        unsafe {
            let entry = qiskit_ffi::qk_target_entry_new_measure();
            for (qargs, [duration, error]) in error_duration_map {
                // TODO make this live long enough
                let mut local_qargs = qargs;
                qiskit_ffi::qk_target_entry_add_property(
                    entry,
                    &mut local_qargs as *mut _,
                    1,
                    duration.unwrap_or(f64::NAN),
                    error.unwrap_or(f64::NAN),
                );
            }
            qiskit_ffi::qk_target_add_instruction(self.0, entry);
        }
    }

    pub fn add_reset(&mut self, error_duration_map: impl Iterator<Item = (u32, [Option<f64>; 2])>) {
        unsafe {
            let entry = qiskit_ffi::qk_target_entry_new_reset();
            for (qargs, [duration, error]) in error_duration_map {
                // TODO make this live long enough
                let mut local_qargs = qargs;
                qiskit_ffi::qk_target_entry_add_property(
                    entry,
                    &mut local_qargs as *mut _,
                    1,
                    duration.unwrap_or(f64::NAN),
                    error.unwrap_or(f64::NAN),
                );
            }
            qiskit_ffi::qk_target_add_instruction(self.0, entry);
        }
    }
}
