// (C) Copyright Matthew Treinish & Kevin Hartman 2025
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

#include "qiskit_ibm_runtime.h"

int main(int argc, char *arv[]) {
    // Build a 5 qubit GHZ state
    QkCircuit *qc = qk_circuit_new(5, 5);
    uint32_t h_qargs[1] = {0, };
    qk_circuit_gate(qc, QkGate_H, h_qargs, NULL);

    for (int i = 1; i < 5; i++) {
        uint32_t qubits[2] = {0, i};
        qk_circuit_gate(qc, QkGate_CX, qubits, NULL);
    }
    for (int i = 0; i < 5; i++) {
        qk_circuit_measure(qc, i, i);
    }

    // Access IQP and select backend
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
    printf("selected backed: %llu", &selected_backend);

    // Transpile circuit for backend
    QkTarget *target = qkrt_get_backend_target(service, backends[selected_backend]);
    QkTranspileResult transpile_result = {NULL, NULL};
    char *error = NULL;
    QkTranspileOptions options = qk_transpiler_default_options();
    options.seed = 42;
    int result_code = qk_transpile(qc, target, &options, &transpile_result, &error);
    if (result_code != 0) {
        printf("transpilation failed with: %s", error);
        goto cleanup_transpile;
    }
    // Run Job on backend 
    int32_t shots = 10000;
    Job *job;
    res = qkrt_sampler_job_run(&job, service, backends[selected_backend], transpile_result.circuit, shots, NULL);
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

    cleanup_search:
    qkrt_backend_search_results_free(results);
    cleanup_transpile:
    qk_circuit_free(transpile_result.circuit);
    qk_transpile_layout_free(transpile_result.layout);
    cleanup_target:
    qk_target_free(target);
    cleanup_service:
    qkrt_service_free(service);
    cleanup:
    qk_circuit_free(qc);
}
