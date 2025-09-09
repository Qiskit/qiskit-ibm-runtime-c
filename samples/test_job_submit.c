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

#include <qiskit.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <qiskit_ibm_runtime/qiskit_ibm_runtime.h>

typedef struct Service Service;
typedef struct Job Job;
typedef struct Backend Backend;
typedef struct BackendSearchResults BackendSearchResults;

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

    int res = 0;

    Service *service;
    res = qkrt_service_new(&service);
    if (res != 0) {
        printf("service new failed with code: %d\n", res);
        goto cleanup;
    }

    BackendSearchResults *results;
    res = qkrt_backend_search(&results, service);
    if (res != 0) {
        printf("backend search failed with code: %d\n", res);
        goto cleanup_service;
    }

    uint64_t result_count = qkrt_backend_search_results_length(results);
    Backend **backends = qkrt_backend_search_results_data(results);
    printf("found backends:\n");
    for (uint64_t i = 0; i < result_count; i++) {
        printf("  [%llu] %s (%s)\n", i, qkrt_backend_name(backends[i]), qkrt_backend_instance_name(backends[i]));
    }

    Backend *least_busy = qkrt_backend_search_results_least_busy(results);
    printf("\nthe least busy is: %s (%s)", qkrt_backend_name(least_busy), qkrt_backend_instance_name(least_busy));

    uint64_t selected_backend = 0;
    printf("\nenter the index of the backend to select: ");
    scanf("%llu", &selected_backend);

    printf("\nyou have selected backend: %s\n", qkrt_backend_name(backends[selected_backend]));

    Job *job;
    res = qkrt_sampler_job_run(&job, service, backends[selected_backend], qc, shots, NULL);
    if (res != 0) {
        printf("job submit failed with code: %d\n", res);
        goto cleanup_search;
    }

    printf("job submit successful!\n");

    uint32_t status;
    do {
        printf("waiting 20 seconds to poll...\n");
        sleep(20);
        res = qkrt_job_status(&status, service, job);
        if (res != 0) {
            printf("status poll failed with code: %d\n", res);
            goto cleanup;
        }
        printf("current status: %d\n", status);
    } while (status == 0 || status == 1);
    printf("job terminated with status: %d\n", status);

    qkrt_job_free(job);
    QkOpCounts op_counts = qk_circuit_count_ops(qc);
    for (int i = 0; i < op_counts.len; i++) {
        printf("%s: %lu\n", op_counts.data[i].name, op_counts.data[i].count);
    }

    cleanup_search:
    qkrt_backend_search_results_free(results);
    cleanup_service:
    qkrt_service_free(service);
    cleanup:
    qk_circuit_free(qc);
}
