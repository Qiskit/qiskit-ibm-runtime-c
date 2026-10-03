// This code is part of Qiskit.
//
// (C) Copyright IBM 2025
//
// This code is licensed under the Apache License, Version 2.0. You may
// obtain a copy of this license in the LICENSE.txt file in the root directory
// of this source tree or at http://www.apache.org/licenses/LICENSE-2.0.
//
// Any modifications or derivative works of this code must retain this
// copyright notice, and modified files need to carry a notice indicating
// that they have been altered from the originals.

// Generate a test qpy file. This must be a static isa circuit
// otherwise it will panic.

#include <qiskit.h>

extern int32_t qkrt_sampler_job_write_payload(QkCircuit *circuit, int32_t shots,
                                              char *backend,
                                              char *runtime,
                                              char *filename,
                                              const char *const *tags);


extern void generate_qpy(QkCircuit *circuit, char *filename);

int main(int argc, char* arv[]) {
    QkCircuit *qc = qk_circuit_new(200, 200);
    for(int i = 0; i < 1000; i++) {
        for(int j = 0; j<200; j+=2) {
            uint32_t qubits[2] = {j, j + 1};
            qk_circuit_gate(qc, QkGate_CZ, qubits, NULL);
        }
    }
    for(int i = 0; i < 200; i++) {
        qk_circuit_measure(qc, i, i);
    }
    static const char *const tags[] = {"tag1", "tag2", NULL};
    qkrt_sampler_job_write_payload(qc, 1024, "ibm_backend_name", "the_runtime", "test_tags.qpy", tags);
    generate_qpy(qc, "test.qpy");
}
