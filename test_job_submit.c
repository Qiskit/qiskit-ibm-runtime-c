// Generate a job create payload to be submitted with curl. This must be a
// static isa circuit otherwise it will panic.

#include <qiskit.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct Job Job;

extern int32_t qkrt_sampler_job_run(Job **job, QkCircuit *circuit, int32_t shots, char *runtime);

extern int32_t qkrt_job_status(uint32_t *status, Job *job);

extern void qkrt_job_free(Job *job);

extern void generate_qpy(QkCircuit *circuit, char *filename);

extern void get_access_token(void);

extern void get_backend_names(void);

int main(int argc, char *arv[]) {
    get_backend_names();
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

    Job *job;
    int res = qkrt_sampler_job_run(&job, qc, shots, NULL);
    if (res != 0) {
        printf("run failed with code: %d\n", res);
        goto cleanup;
    }

    uint32_t status;
    do {
        res = qkrt_job_status(&status, job);
        if (res != 0) {
            printf("status poll failed with code: %d\n", res);
            goto cleanup;
        }
        printf("current status: %d\n", status);
    } while (status != 0 && status != 1);
    printf("job terminated with status: %d\n", status);

    qkrt_job_free(job);
    QkOpCounts op_counts = qk_circuit_count_ops(qc);
    for (int i = 0; i < op_counts.len; i++) {
        printf("%s: %lu\n", op_counts.data[i].name, op_counts.data[i].count);
    }
    cleanup:
    qk_circuit_free(qc);
}
