// This code is part of Qiskit.
//
// (C) Copyright IBM 2026
//
// This code is licensed under the Apache License, Version 2.0. You may
// obtain a copy of this license in the LICENSE.txt file in the root directory
// of this source tree or at http://www.apache.org/licenses/LICENSE-2.0.
//
// Any modifications or derivative works of this code must retain this
// copyright notice, and modified files need to carry a notice indicating
// that they have been altered from the originals.

use crate::qiskit_ffi;
use std::collections::HashMap;
use std::ffi::CString;


pub struct BitTerm(pub *mut qiskit_ffi::QkBitTerm);
pub struct SparseObservable(pub *mut qiskit_ffi::QkObs);

impl SparseObservable {
    pub fn num_qubits(&self) -> u32 {
        unsafe { qiskit_ffi::qk_obs_num_qubits(self.0) }
    }

    pub fn num_terms(&self) -> usize {
        unsafe { qiskit_ffi::qk_obs_num_terms(self.0) }
    }

    pub fn to_term_map(&self) -> HashMap<String, f64> {
        let num_terms = self.num_terms();
        let mut term = std::mem::MaybeUninit::<qiskit_ffi::QkObsTerm>::uninit();
        (0..num_terms as u64).map(|i| unsafe {
            qiskit_ffi::qk_obs_term(self.0, i, term.as_mut_ptr());
            let mut label = vec!['I' as u8; self.num_qubits() as usize];
            let term = &*term.as_ptr();
            let bit_terms = std::slice::from_raw_parts(term.bit_terms, term.len);
            let indices = std::slice::from_raw_parts(term.indices, term.len);
            for j in 0..term.len {
                let bit_term = bit_terms[j];
                let index = indices[j];
                label[index as usize] = qiskit_ffi::qk_bitterm_label(bit_term);
            }
            let coeff_re = term.coeff.re;
            let out_str = String::from_utf8_unchecked(label);
            (out_str, coeff_re)
        }).collect()
    }
}
