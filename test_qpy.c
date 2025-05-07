// Generate a test qpy file. This must be a static isa circuit
// otherwise it will panic.

#include <qiskit.h>

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
    generate_qpy(qc, "test.qpy");
}
