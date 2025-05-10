// Generate a job create payload to be submitted with curl. This must be a
// static isa circuit otherwise it will panic.

#include <qiskit.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern void generate_qpy(QkCircuit *circuit, char *filename);
extern void save_sampler_job_payload(QkCircuit *circuit, int32_t shots,
                                     char *backend, char *runtime,
                                     char *filename);

int main(int argc, char *arv[]) {
  QkCircuit *qc = qk_circuit_new(100, 100);
  uint32_t qubits[1] = {
      0,
  };
  double params[1] = {3.14159};
  qk_circuit_gate(qc, QkGate_RZ, qubits, params);
  qk_circuit_gate(qc, QkGate_SX, qubits, NULL);
  qk_circuit_gate(qc, QkGate_RZ, qubits, params);
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 101; j += 1) {
      uint32_t qubits[2] = {0, 1};
      qk_circuit_gate(qc, QkGate_CZ, qubits, NULL);
    }
  }
  for (int i = 0; i < 100; i++) {
    qk_circuit_measure(qc, i, i);
  }
  int32_t shots = 4196;
  generate_qpy(qc, "test_before_json.qpy");
  save_sampler_job_payload(qc, shots, "ibm_pittsburgh", NULL, "test_pitt.json");
  QkOpCounts op_counts = qk_circuit_count_ops(qc);
  for (int i = 0; i < op_counts.len; i++) {
    printf("%s: %d\n", op_counts.data[i].name, op_counts.data[i].count);
  }
  qk_circuit_free(qc);
}
