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

#include <qiskit_ibm_runtime/qiskit_ibm_runtime.h>

void build_lucj(QkCircuit *qc);

int main(int argc, char *arv[]) {
    // Build an lucj circuit for fe4s4
    QkCircuit *qc = qk_circuit_new(72, 72);
    build_lucj(qc);
    for (uint32_t i = 0; i < qk_circuit_num_qubits(qc); i++) {
        qk_circuit_measure(qc, i, i);
    }
    // Access IBM Quantum Platform and prompt to select a backend
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
    printf("selected backed: %s\n", qkrt_backend_name(backends[selected_backend]));

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
    
    // Submit a sampler job with transpiled circuit on backend
    int32_t shots = 10000;
    Job *job;
    res = qkrt_sampler_job_run(&job, service, backends[selected_backend], transpile_result.circuit, shots, NULL);
    if (res != 0) {
        printf("job submit failed with code: %d\n", res);
        goto cleanup_search;
    }
    printf("job submit successful!\n");
    // Poll until complete
    uint32_t status;
    do {
        printf("waiting 20 seconds to poll...\n");
        sleep(20);
        res = qkrt_job_status(&status, service, job);
        if (res != 0) {
            printf("status poll failed with code: %d\n", res);
            goto cleanup;
        }
        if (status == 0) {
            printf("current status: queued\n");
        } else if (status == 1) {
            printf("current status: in progress\n");
        }
    } while (status == 0 || status == 1);
    if (status == 2) {
        printf("Job completed\n");
    } else {
        printf("job terminated with status: %d\n", status);
    }
    // Get first bitstring result
    Samples *samples;
    res = qkrt_job_results(&samples, service, job);
    printf("Job has %d samples\nThe first sample is:\n", qkrt_samples_num_samples(samples));
    char *first_sample = qkrt_samples_get_sample(samples, 0);
    printf("%s\n", first_sample);
    qkrt_str_free(first_sample);
    qkrt_samples_free(samples);

cleanup_job:
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

void build_lucj(QkCircuit *qc) {
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        0,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        1,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        2,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        3,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        4,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        5,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        6,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        7,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        8,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        9,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        10,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        11,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        12,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        13,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        14,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        15,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        16,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        17,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        18,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        19,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        20,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        21,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        22,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        23,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        24,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        25,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        26,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        26,
                    },
                    (double[]){
                        1.5707963267948901,
                        -4.712388980384686,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        25,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        24,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        23,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        22,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        21,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        20,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        19,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        18,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        17,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        16,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        15,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        1,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        0,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        27,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        26,
                    },
                    (double[]){
                        1.5707963267948908,
                        -1.5707963267949,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        25,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        24,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        23,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        22,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        21,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        20,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        19,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        18,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        17,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        16,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        15,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        1,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        0,
                        1,
                    },
                    (double[]){
                        -0.1313897706371172,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        28,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        27,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        26,
                    },
                    (double[]){
                        1.5707963267948946,
                        -4.712388980384689,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        25,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        24,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        23,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        22,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        21,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        20,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        19,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        18,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        17,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        16,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        15,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        1,
                        2,
                    },
                    (double[]){
                        -0.025916562177373044,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        29,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        28,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        27,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        26,
                    },
                    (double[]){
                        1.5707963267948961,
                        -1.5707963267948946,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        25,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        24,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        23,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        22,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        21,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        20,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        19,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        18,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        17,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        16,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        15,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        2,
                        3,
                    },
                    (double[]){
                        -0.011516021679808216,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        30,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        29,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        28,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        27,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        26,
                    },
                    (double[]){
                        1.570796326794894,
                        1.5707963267948957,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        25,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        24,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        23,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        22,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        21,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        20,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        19,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        18,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        17,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        16,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        15,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        3,
                        4,
                    },
                    (double[]){
                        -0.007921166010645924,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        0.5413958818868657,
                        -3.5217785396477357,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        31,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        30,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        29,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        28,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        27,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        26,
                    },
                    (double[]){
                        1.5707963267948877,
                        -1.570796326794892,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        25,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        24,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        23,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        22,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        21,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        20,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        19,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        18,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        17,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        16,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        15,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        4,
                        5,
                    },
                    (double[]){
                        -0.0061687747756473755,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        32,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        31,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        30,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        29,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        28,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        27,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        26,
                    },
                    (double[]){
                        1.5707963267948912,
                        -4.712388980384688,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        25,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        24,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        23,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        22,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        21,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        20,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        19,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        18,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        17,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        16,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        15,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        5,
                        6,
                    },
                    (double[]){
                        -0.004889240780821784,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        33,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        32,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        31,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        30,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        29,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        28,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        27,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        26,
                    },
                    (double[]){
                        1.5707963267948943,
                        1.5707963267948886,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        25,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        24,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        23,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        22,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        21,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        20,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        19,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        18,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        17,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        16,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        15,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        6,
                        7,
                    },
                    (double[]){
                        -0.001735199881523165,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        35,
                        34,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        33,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        32,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        31,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        30,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        29,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        28,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        27,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        26,
                    },
                    (double[]){
                        1.5707963267948974,
                        -1.570796326794895,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        25,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        24,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        23,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        22,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        21,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        20,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        19,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        18,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        17,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        16,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        15,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        7,
                        8,
                    },
                    (double[]){
                        -0.00030530715300445936,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        0.16585751622923148,
                        -2.8875394524759206,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        8,
                        9,
                    },
                    (double[]){
                        -3.0249353513152624e-18,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        9,
                        10,
                    },
                    (double[]){
                        -4.3239784490922713e-32,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        10,
                        11,
                    },
                    (double[]){
                        -2.7049864477076644e-32,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        11,
                        12,
                    },
                    (double[]){
                        -1.3856276195844522e-32,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        2.7346280565016583,
                        -2.247698839827188,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        12,
                        13,
                    },
                    (double[]){
                        -5.693727223139431e-33,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        13,
                        14,
                    },
                    (double[]){
                        -3.4904555310094395e-33,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        14,
                        15,
                    },
                    (double[]){
                        -1.9516349326242628e-33,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        15,
                        16,
                    },
                    (double[]){
                        -5.323955964695674e-34,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        1.126846529913875,
                        -0.6244654068040163,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        16,
                        17,
                    },
                    (double[]){
                        3.1221431943649245e-35,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        17,
                        18,
                    },
                    (double[]){
                        -2.9099188577375687e-35,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        18,
                        19,
                    },
                    (double[]){
                        -2.9634784911515096e-34,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        19,
                        20,
                    },
                    (double[]){
                        -9.307395205580684e-34,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        18,
                    },
                    (double[]){
                        1.3297090774195535,
                        -2.479092017641391,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        20,
                        21,
                    },
                    (double[]){
                        -3.09409280521893e-33,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        21,
                        22,
                    },
                    (double[]){
                        -6.432320597451137e-33,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        20,
                    },
                    (double[]){
                        1.9763117760147517,
                        -2.3791469089304664,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        19,
                    },
                    (double[]){
                        1.4829369883727135,
                        -3.176001210796275,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        22,
                        23,
                    },
                    (double[]){
                        -8.503432557401052e-33,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        23,
                        24,
                    },
                    (double[]){
                        -1.036771669421122e-32,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        22,
                    },
                    (double[]){
                        0.568178689564719,
                        -2.7875700805525585,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        21,
                    },
                    (double[]){
                        2.190318552544644,
                        -1.9361426486997388,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        20,
                    },
                    (double[]){
                        1.9460534868347654,
                        -2.212226455943384,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        24,
                        25,
                    },
                    (double[]){
                        -1.4403737631602665e-32,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        25,
                        26,
                    },
                    (double[]){
                        -3.801587985268212e-32,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        24,
                    },
                    (double[]){
                        2.2442526358950934,
                        -2.589261115521901,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        23,
                    },
                    (double[]){
                        1.823323060273371,
                        1.0135956007942357,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        22,
                    },
                    (double[]){
                        2.0786204936832604,
                        0.17695201530542026,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        21,
                    },
                    (double[]){
                        2.8560631090881383,
                        0.5177388285764155,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        26,
                        27,
                    },
                    (double[]){
                        -3.3622446243837795e-18,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        27,
                        28,
                    },
                    (double[]){
                        -0.00030530715300446147,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        26,
                    },
                    (double[]){
                        1.1485744501587734,
                        -2.292734797535991,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        25,
                    },
                    (double[]){
                        0.9575446826239236,
                        -3.465587386063616,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        24,
                    },
                    (double[]){
                        2.4620925734551538,
                        1.5155400360600368,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        23,
                    },
                    (double[]){
                        2.2749539553785083,
                        -4.6863172020933455,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        22,
                    },
                    (double[]){
                        2.9606687428227003,
                        -2.373991477669906,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        28,
                        29,
                    },
                    (double[]){
                        -0.001735199881523166,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        29,
                        30,
                    },
                    (double[]){
                        -0.004889240780821781,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        28,
                    },
                    (double[]){
                        1.95880434190825,
                        -0.1873123709786897,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        27,
                    },
                    (double[]){
                        2.0449703502619827,
                        0.36032937222223316,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        26,
                    },
                    (double[]){
                        2.375376357398631,
                        1.3769272052053392,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        25,
                    },
                    (double[]){
                        2.8710372113916476,
                        0.9632669138922791,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        24,
                    },
                    (double[]){
                        2.262586008727154,
                        -1.288370362859426,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        23,
                    },
                    (double[]){
                        2.3304866455186426,
                        -2.180128020851477,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        30,
                        31,
                    },
                    (double[]){
                        -0.0061687747756473685,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        31,
                        32,
                    },
                    (double[]){
                        -0.007921166010645917,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        30,
                    },
                    (double[]){
                        1.3952661967326057,
                        1.5430907982275288,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        29,
                    },
                    (double[]){
                        2.4586706200703428,
                        -4.410828153607344,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        28,
                    },
                    (double[]){
                        2.4913044734413945,
                        0.8713268735802138,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        27,
                    },
                    (double[]){
                        1.666532151939135,
                        1.565665605313919,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        26,
                    },
                    (double[]){
                        2.230091520874786,
                        -1.4649062073220245,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        25,
                    },
                    (double[]){
                        2.9230761214729517,
                        0.6183467120914266,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        24,
                    },
                    (double[]){
                        2.5800551498051942,
                        -1.8572280415164675,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        32,
                        33,
                    },
                    (double[]){
                        -0.011516021679808202,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        33,
                        34,
                    },
                    (double[]){
                        -0.025916562177373068,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        32,
                    },
                    (double[]){
                        1.6840267276395249,
                        1.3746712052149852,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        31,
                    },
                    (double[]){
                        1.8669601257467952,
                        -2.4444792677705824,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        30,
                    },
                    (double[]){
                        1.8301383984801725,
                        0.22910785102960962,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        29,
                    },
                    (double[]){
                        2.7311693460833824,
                        -3.6583006939931435,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        28,
                    },
                    (double[]){
                        2.1260114700692845,
                        1.4086044909515887,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        27,
                    },
                    (double[]){
                        1.8850080179093571,
                        -0.9650941623735478,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        26,
                    },
                    (double[]){
                        2.249214594410175,
                        -2.4803284947061406,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        25,
                    },
                    (double[]){
                        2.6155650341156544,
                        -2.205981649883237,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        34,
                        35,
                    },
                    (double[]){
                        -0.13138977063711743,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        35,
                        34,
                    },
                    (double[]){
                        2.9425523988971585,
                        1.244459690259606,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        33,
                    },
                    (double[]){
                        2.5977548969047333,
                        0.5116690534924846,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        32,
                    },
                    (double[]){
                        1.4687175815231555,
                        0.9195661617661721,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        31,
                    },
                    (double[]){
                        2.5966094707827123,
                        -3.841809837929219,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        30,
                    },
                    (double[]){
                        2.857606667943773,
                        0.2922107924779702,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        29,
                    },
                    (double[]){
                        2.217134679040776,
                        -4.384792148943332,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        28,
                    },
                    (double[]){
                        2.3606467022857913,
                        -3.317246580699604,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        27,
                    },
                    (double[]){
                        2.524675909472037,
                        -3.8871984509465953,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        26,
                    },
                    (double[]){
                        2.647370764930284,
                        -0.6606543668337115,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        35,
                    },
                    (double[]){
                        2.0210641802042426,
                        -0.551123437114351,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        34,
                    },
                    (double[]){
                        2.087775029547971,
                        -2.7500858506231873,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        33,
                    },
                    (double[]){
                        2.1184565902501484,
                        -3.8693632019117348,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        32,
                    },
                    (double[]){
                        2.5805046941317165,
                        -0.5812836528828598,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        31,
                    },
                    (double[]){
                        2.650703706279322,
                        -0.44477955355608745,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        30,
                    },
                    (double[]){
                        2.056191870394667,
                        -1.9787869019209845,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        29,
                    },
                    (double[]){
                        2.590049569273978,
                        -2.591237663381828,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        28,
                    },
                    (double[]){
                        3.041007499237274,
                        -0.19363824945661934,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        35,
                    },
                    (double[]){
                        1.533542493279316,
                        -3.7479532954880965,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        34,
                    },
                    (double[]){
                        2.4726061902963212,
                        -1.3221128190269889,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        33,
                    },
                    (double[]){
                        2.8991394271685422,
                        0.967069086431434,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        32,
                    },
                    (double[]){
                        2.4012311762556884,
                        -4.395089074120078,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        31,
                    },
                    (double[]){
                        2.7777596954336636,
                        -2.3033070755442733,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        30,
                    },
                    (double[]){
                        2.583345204168922,
                        -0.4998945275083453,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        29,
                    },
                    (double[]){
                        2.6438460268628843,
                        -0.05393188300760898,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        35,
                    },
                    (double[]){
                        1.7038068755110596,
                        -0.6243514863436643,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        34,
                    },
                    (double[]){
                        2.004763129741312,
                        -1.9184020066944294,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        33,
                    },
                    (double[]){
                        2.5649194591236775,
                        -1.5833973286830174,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        32,
                    },
                    (double[]){
                        2.6684606183552666,
                        -2.912209056266535,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        31,
                    },
                    (double[]){
                        2.5713681923008185,
                        1.1975507754839918,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        30,
                    },
                    (double[]){
                        2.9379639216410274,
                        -0.720873854234127,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        35,
                    },
                    (double[]){
                        1.7536327706080703,
                        0.8936871601852054,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        34,
                    },
                    (double[]){
                        2.6033331883323743,
                        -1.3247631881996476,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        33,
                    },
                    (double[]){
                        2.632336287334508,
                        1.3306789845174936,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        32,
                    },
                    (double[]){
                        2.521098055694825,
                        1.4611841682738578,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        31,
                    },
                    (double[]){
                        2.7066268950849746,
                        -0.02824881381915989,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        35,
                    },
                    (double[]){
                        1.6908574051152372,
                        -0.32626491071549113,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        34,
                    },
                    (double[]){
                        1.596667865950821,
                        -2.6555932404886655,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        33,
                    },
                    (double[]){
                        2.8709162748558796,
                        -3.7857927744684448,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        32,
                    },
                    (double[]){
                        2.7781822561835328,
                        -3.8457711354452107,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        35,
                    },
                    (double[]){
                        2.972440719308561,
                        -0.574456083783246,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        34,
                    },
                    (double[]){
                        2.723537763406409,
                        -0.19587625915704798,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        33,
                    },
                    (double[]){
                        2.3587844421592323,
                        1.1362268380479352,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        35,
                    },
                    (double[]){
                        1.8752961191630328,
                        0.5980980375219334,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        34,
                    },
                    (double[]){
                        0.8897649234049255,
                        -0.5290590326528712,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        35,
                    },
                    (double[]){
                        1.7643645831398382,
                        -3.86609532141712,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        36,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        37,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        38,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        39,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        40,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        41,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        42,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        43,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        44,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        45,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        46,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        47,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        48,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        49,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        50,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        51,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        52,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        53,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        54,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        55,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        56,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        57,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        58,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        59,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_X,
                    (uint32_t[]){
                        60,
                    },
                    NULL);
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        61,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        62,
                    },
                    (double[]){
                        3.141592653589793,
                        -3.141592653589793,
                        0.0,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        62,
                    },
                    (double[]){
                        1.5707963267948901,
                        -4.712388980384686,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        61,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        60,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        59,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        58,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        57,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        56,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        55,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        54,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        53,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        52,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        51,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        37,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        36,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        63,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        62,
                    },
                    (double[]){
                        1.5707963267948908,
                        -1.5707963267949,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        61,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        60,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        59,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        58,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        57,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        56,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        55,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        54,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        53,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        52,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        51,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        37,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        36,
                        37,
                    },
                    (double[]){
                        -0.1313897706371172,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        0,
                        36,
                    },
                    (double[]){
                        -0.3916757868860434,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        0,
                    },
                    (double[]){
                        1.1045591850745158,
                        1.3414234870215682,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        1,
                    },
                    (double[]){
                        1.7224934047917944,
                        -1.610874495224318,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        0,
                    },
                    (double[]){
                        2.6708034219646506,
                        -3.387926402420276,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        64,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        63,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        62,
                    },
                    (double[]){
                        1.5707963267948946,
                        -4.712388980384689,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        61,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        60,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        59,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        58,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        57,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        56,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        55,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        54,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        53,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        52,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        51,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        37,
                        38,
                    },
                    (double[]){
                        -0.025916562177373044,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        36,
                    },
                    (double[]){
                        1.1045591850745158,
                        1.3414234870215682,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        65,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        64,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        63,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        62,
                    },
                    (double[]){
                        1.5707963267948961,
                        -1.5707963267948946,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        61,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        60,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        59,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        58,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        57,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        56,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        55,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        54,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        53,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        52,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        51,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        38,
                        39,
                    },
                    (double[]){
                        -0.011516021679808216,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        66,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        65,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        64,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        63,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        62,
                    },
                    (double[]){
                        1.570796326794894,
                        1.5707963267948957,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        61,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        60,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        59,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        58,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        57,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        56,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        55,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        54,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        53,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        52,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        51,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        39,
                        40,
                    },
                    (double[]){
                        -0.007921166010645924,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        0.5413958818868657,
                        -3.5217785396477357,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        37,
                    },
                    (double[]){
                        1.7224934047917944,
                        -1.610874495224318,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        36,
                    },
                    (double[]){
                        2.6708034219646506,
                        -3.387926402420276,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        67,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        66,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        65,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        64,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        63,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        62,
                    },
                    (double[]){
                        1.5707963267948877,
                        -1.570796326794892,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        61,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        60,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        59,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        58,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        57,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        56,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        55,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        54,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        53,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        52,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        51,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        40,
                        41,
                    },
                    (double[]){
                        -0.0061687747756473755,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        4,
                        40,
                    },
                    (double[]){
                        -0.007209940530513806,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        1.2210227241299427,
                        -0.8831875223244728,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        1.2985640829244132,
                        -4.583772143911702,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        1.834257734030856,
                        1.392166778548428,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        1,
                    },
                    (double[]){
                        1.713741377834872,
                        -3.4585191858066144,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        0,
                    },
                    (double[]){
                        2.3060352232210013,
                        0.6802191442961223,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        2.8455818110981608,
                        -4.159667241495841,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        2.728359845747492,
                        -1.9364713725302873,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        2.12556535397144,
                        -4.676386106073656,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        2.988885362933094,
                        0.6056673197314528,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        1,
                    },
                    (double[]){
                        2.1812980269114903,
                        -1.1593891465206694,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        0,
                    },
                    (double[]){
                        1.1414651550120534,
                        -3.74851446917017,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        68,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        67,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        66,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        65,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        64,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        63,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        62,
                    },
                    (double[]){
                        1.5707963267948912,
                        -4.712388980384688,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        61,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        60,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        59,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        58,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        57,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        56,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        55,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        54,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        53,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        52,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        51,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        41,
                        42,
                    },
                    (double[]){
                        -0.004889240780821784,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        1.2210227241299427,
                        -0.8831875223244728,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        1.2985640829244132,
                        -4.583772143911702,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        1.834257734030856,
                        1.392166778548428,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        37,
                    },
                    (double[]){
                        1.713741377834872,
                        -3.4585191858066144,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        36,
                    },
                    (double[]){
                        2.3060352232210013,
                        0.6802191442961223,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        69,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        68,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        67,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        66,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        65,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        64,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        63,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        62,
                    },
                    (double[]){
                        1.5707963267948943,
                        1.5707963267948886,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        61,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        60,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        59,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        58,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        57,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        56,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        55,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        54,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        53,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        52,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        51,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        42,
                        43,
                    },
                    (double[]){
                        -0.001735199881523165,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        71,
                        70,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        69,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        68,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        67,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        66,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        65,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        64,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        63,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        62,
                    },
                    (double[]){
                        1.5707963267948974,
                        -1.570796326794895,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        61,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        60,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        59,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        58,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        57,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        56,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        55,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        54,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        53,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        52,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        51,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        3.141592653589793,
                        -1.5707963267948966,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        43,
                        44,
                    },
                    (double[]){
                        -0.00030530715300445936,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        0.16585751622923148,
                        -2.8875394524759206,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        2.8455818110981608,
                        -4.159667241495841,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        2.728359845747492,
                        -1.9364713725302873,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        2.12556535397144,
                        -4.676386106073656,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        2.988885362933094,
                        0.6056673197314528,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        37,
                    },
                    (double[]){
                        2.1812980269114903,
                        -1.1593891465206694,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        36,
                    },
                    (double[]){
                        1.1414651550120534,
                        -3.74851446917017,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        44,
                        45,
                    },
                    (double[]){
                        -3.0249353513152624e-18,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        45,
                        46,
                    },
                    (double[]){
                        -4.3239784490922713e-32,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        46,
                        47,
                    },
                    (double[]){
                        -2.7049864477076644e-32,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        47,
                        48,
                    },
                    (double[]){
                        -1.3856276195844522e-32,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        2.7346280565016583,
                        -2.247698839827188,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        48,
                        49,
                    },
                    (double[]){
                        -5.693727223139431e-33,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        12,
                        48,
                    },
                    (double[]){
                        -7.517775766757048e-33,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        2.1715828084653155,
                        -2.7225564748992173,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        2.4871470385761896,
                        -4.310531164728653,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        1.9997016990187038,
                        -4.434822856436874,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        2.8113929185589397,
                        -0.9278125773272227,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        49,
                        50,
                    },
                    (double[]){
                        -3.4904555310094395e-33,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        2.1715828084653155,
                        -2.7225564748992173,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        2.4871470385761896,
                        -4.310531164728653,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        50,
                        51,
                    },
                    (double[]){
                        -1.9516349326242628e-33,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        51,
                        52,
                    },
                    (double[]){
                        -5.323955964695674e-34,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        1.126846529913875,
                        -0.6244654068040163,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        1.9997016990187038,
                        -4.434822856436874,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        2.8113929185589397,
                        -0.9278125773272227,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        52,
                        53,
                    },
                    (double[]){
                        3.1221431943649245e-35,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        16,
                        52,
                    },
                    (double[]){
                        -2.102480691163263e-34,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        16,
                    },
                    (double[]){
                        0.26005679742621574,
                        -3.0492013701463936,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        15,
                    },
                    (double[]){
                        2.073669917277358,
                        0.46993785359407214,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        2.104553939938901,
                        -4.709800036858085,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        2.9363474217170915,
                        -0.22412439411163376,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        17,
                    },
                    (double[]){
                        2.237046069827435,
                        -0.9357675081566198,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        16,
                    },
                    (double[]){
                        1.3455922569054257,
                        1.2438817732206844,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        15,
                    },
                    (double[]){
                        2.5530113564719055,
                        -1.3409758951729123,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        2.699974327188605,
                        -2.3719858389472326,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        18,
                    },
                    (double[]){
                        2.902276779649731,
                        -2.2771147868380237,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        17,
                    },
                    (double[]){
                        1.9248706087811391,
                        -0.6002417970200256,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        16,
                    },
                    (double[]){
                        2.730227241485885,
                        -3.1180356210205806,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        15,
                    },
                    (double[]){
                        2.8233825476499956,
                        -2.443735365814959,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        19,
                    },
                    (double[]){
                        2.9402534913147287,
                        1.0327856672995814,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        18,
                    },
                    (double[]){
                        1.6690226619915483,
                        -3.805945095474075,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        17,
                    },
                    (double[]){
                        2.371765935282514,
                        1.058960267140232,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        16,
                    },
                    (double[]){
                        2.084590743872719,
                        -1.8107473308413788,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        20,
                    },
                    (double[]){
                        2.718296685379003,
                        -2.3951508094273004,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        19,
                    },
                    (double[]){
                        2.5019425526838046,
                        0.15978169332224068,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        18,
                    },
                    (double[]){
                        2.7248379914965355,
                        1.5377722064389343,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        17,
                    },
                    (double[]){
                        2.8011928354990574,
                        -0.23359195611402783,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        21,
                    },
                    (double[]){
                        2.907692651101327,
                        -1.4369155154395827,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        20,
                    },
                    (double[]){
                        2.3395273213007,
                        -0.33748469818491644,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        19,
                    },
                    (double[]){
                        2.7089503985645282,
                        -0.6835159594911604,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        18,
                    },
                    (double[]){
                        2.5751254442413387,
                        -2.163338465066995,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        22,
                    },
                    (double[]){
                        2.9353609066189064,
                        -3.3001689532025207,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        21,
                    },
                    (double[]){
                        2.6569427995689936,
                        -0.2880230591158375,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        20,
                    },
                    (double[]){
                        2.8972242507940784,
                        0.6606327047745335,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        19,
                    },
                    (double[]){
                        2.637578306224484,
                        -0.1297496490111094,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        23,
                    },
                    (double[]){
                        2.882691716351001,
                        0.7527172167199372,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        22,
                    },
                    (double[]){
                        2.4450464342021716,
                        -3.6183762373801622,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        21,
                    },
                    (double[]){
                        2.817425709036258,
                        -1.1910839927076715,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        20,
                    },
                    (double[]){
                        2.775298602477821,
                        -3.2138698482015826,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        24,
                    },
                    (double[]){
                        2.778968709622577,
                        -0.33626981631042474,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        23,
                    },
                    (double[]){
                        2.5186210453057445,
                        0.720163020649025,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        22,
                    },
                    (double[]){
                        2.388143234823213,
                        -3.4976489268925866,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        21,
                    },
                    (double[]){
                        2.527133442038302,
                        -1.33481245875564,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        25,
                    },
                    (double[]){
                        2.833740212280427,
                        -2.6981050568896148,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        24,
                    },
                    (double[]){
                        2.6071332045159634,
                        -0.030583533201665514,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        23,
                    },
                    (double[]){
                        3.0037194397026665,
                        -2.8932672727997018,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        22,
                    },
                    (double[]){
                        2.903305332662455,
                        -3.578931423157311,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        27,
                    },
                    (double[]){
                        2.404272376757764,
                        1.0531277859521406,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        26,
                    },
                    (double[]){
                        2.649632527416618,
                        -3.0388033668880587,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        25,
                    },
                    (double[]){
                        2.9041620654312825,
                        -1.5416185594488774,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        24,
                    },
                    (double[]){
                        2.444510836099629,
                        -0.10942558713670758,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        28,
                    },
                    (double[]){
                        2.7943633732662674,
                        -2.4081597994340584,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        27,
                    },
                    (double[]){
                        2.974342923961377,
                        -4.4379877419307725,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        26,
                    },
                    (double[]){
                        2.926509629789757,
                        -4.452286722683915,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        25,
                    },
                    (double[]){
                        2.8580796594271427,
                        1.2730365118080633,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        29,
                    },
                    (double[]){
                        2.477562700881187,
                        -1.1393338971398763,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        28,
                    },
                    (double[]){
                        2.6297181868801864,
                        1.2235048982973615,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        27,
                    },
                    (double[]){
                        3.0058250746759203,
                        -3.2311610620634603,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        26,
                    },
                    (double[]){
                        2.684126721032168,
                        -4.591300612116007,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        30,
                    },
                    (double[]){
                        2.8607366954880113,
                        -2.576894264373272,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        29,
                    },
                    (double[]){
                        2.8199273789059625,
                        0.9147556964154977,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        28,
                    },
                    (double[]){
                        2.787286454096322,
                        -1.7507957281446211,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        27,
                    },
                    (double[]){
                        2.7477718965437687,
                        -3.123114100474603,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        31,
                    },
                    (double[]){
                        2.4803897860056856,
                        -2.4961210243174516,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        30,
                    },
                    (double[]){
                        2.4267145925396347,
                        -0.10408971690399205,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        29,
                    },
                    (double[]){
                        2.571553195663667,
                        -0.2082231811337074,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        28,
                    },
                    (double[]){
                        2.5129411776160424,
                        -4.111576682887643,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        32,
                    },
                    (double[]){
                        1.9984493337912554,
                        -3.390226961994027,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        31,
                    },
                    (double[]){
                        2.3529877699559005,
                        1.2389383797299987,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        30,
                    },
                    (double[]){
                        2.7713007362856614,
                        0.8867841628862325,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        29,
                    },
                    (double[]){
                        2.6430312781134875,
                        0.47757632191837684,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        33,
                    },
                    (double[]){
                        2.9510331485851946,
                        -2.41976204643964,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        32,
                    },
                    (double[]){
                        2.8228569752854598,
                        1.2793405799811803,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        31,
                    },
                    (double[]){
                        3.087747497059217,
                        -3.840205915315935,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        30,
                    },
                    (double[]){
                        2.144689330430432,
                        -2.8452421880548897,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        34,
                    },
                    (double[]){
                        2.3840718968397203,
                        -2.5821911648319045,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        33,
                    },
                    (double[]){
                        2.7067900446961133,
                        -3.8126043823983697,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        32,
                    },
                    (double[]){
                        2.056176729133506,
                        1.42320848780381,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        31,
                    },
                    (double[]){
                        2.9051430380636383,
                        -3.241125849043073,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        35,
                    },
                    (double[]){
                        2.566850714915489,
                        0.011836547825281984,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        34,
                    },
                    (double[]){
                        2.240855013816098,
                        -2.533996036884421,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        33,
                    },
                    (double[]){
                        2.0044155058794657,
                        -2.5790305954051247,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        32,
                    },
                    (double[]){
                        2.4144076119766806,
                        -3.8792820864595945,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        35,
                    },
                    (double[]){
                        1.9755719034098922,
                        -2.737459089474252,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        34,
                    },
                    (double[]){
                        2.1626070452317525,
                        1.0525054273598533,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        33,
                    },
                    (double[]){
                        2.0784423525438185,
                        -2.122379661305918,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        35,
                    },
                    (double[]){
                        2.9033659353537726,
                        1.1136882349587318,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        34,
                    },
                    (double[]){
                        2.2745327725624063,
                        1.1230296871601921,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        35,
                    },
                    (double[]){
                        1.9214687887805006,
                        0.24329186720151852,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        53,
                        54,
                    },
                    (double[]){
                        -2.9099188577375687e-35,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        52,
                    },
                    (double[]){
                        0.26005679742621574,
                        -3.0492013701463936,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        51,
                    },
                    (double[]){
                        2.073669917277358,
                        0.46993785359407214,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        2.104553939938901,
                        -4.709800036858085,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        2.9363474217170915,
                        -0.22412439411163376,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        54,
                        55,
                    },
                    (double[]){
                        -2.9634784911515096e-34,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        55,
                        56,
                    },
                    (double[]){
                        -9.307395205580684e-34,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        54,
                    },
                    (double[]){
                        1.3297090774195535,
                        -2.479092017641391,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        53,
                    },
                    (double[]){
                        2.237046069827435,
                        -0.9357675081566198,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        52,
                    },
                    (double[]){
                        1.3455922569054257,
                        1.2438817732206844,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        51,
                    },
                    (double[]){
                        2.5530113564719055,
                        -1.3409758951729123,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        2.699974327188605,
                        -2.3719858389472326,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        56,
                        57,
                    },
                    (double[]){
                        -3.09409280521893e-33,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        57,
                        58,
                    },
                    (double[]){
                        -6.432320597451137e-33,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        56,
                    },
                    (double[]){
                        1.9763117760147517,
                        -2.3791469089304664,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        55,
                    },
                    (double[]){
                        1.4829369883727135,
                        -3.176001210796275,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        54,
                    },
                    (double[]){
                        2.902276779649731,
                        -2.2771147868380237,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        53,
                    },
                    (double[]){
                        1.9248706087811391,
                        -0.6002417970200256,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        52,
                    },
                    (double[]){
                        2.730227241485885,
                        -3.1180356210205806,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        51,
                    },
                    (double[]){
                        2.8233825476499956,
                        -2.443735365814959,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        58,
                        59,
                    },
                    (double[]){
                        -8.503432557401052e-33,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        59,
                        60,
                    },
                    (double[]){
                        -1.036771669421122e-32,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        58,
                    },
                    (double[]){
                        0.568178689564719,
                        -2.7875700805525585,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        57,
                    },
                    (double[]){
                        2.190318552544644,
                        -1.9361426486997388,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        56,
                    },
                    (double[]){
                        1.9460534868347654,
                        -2.212226455943384,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        55,
                    },
                    (double[]){
                        2.9402534913147287,
                        1.0327856672995814,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        54,
                    },
                    (double[]){
                        1.6690226619915483,
                        -3.805945095474075,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        53,
                    },
                    (double[]){
                        2.371765935282514,
                        1.058960267140232,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        52,
                    },
                    (double[]){
                        2.084590743872719,
                        -1.8107473308413788,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        60,
                        61,
                    },
                    (double[]){
                        -1.4403737631602665e-32,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        61,
                        62,
                    },
                    (double[]){
                        -3.801587985268212e-32,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        60,
                    },
                    (double[]){
                        2.2442526358950934,
                        -2.589261115521901,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        59,
                    },
                    (double[]){
                        1.823323060273371,
                        1.0135956007942357,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        58,
                    },
                    (double[]){
                        2.0786204936832604,
                        0.17695201530542026,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        57,
                    },
                    (double[]){
                        2.8560631090881383,
                        0.5177388285764155,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        56,
                    },
                    (double[]){
                        2.718296685379003,
                        -2.3951508094273004,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        55,
                    },
                    (double[]){
                        2.5019425526838046,
                        0.15978169332224068,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        54,
                    },
                    (double[]){
                        2.7248379914965355,
                        1.5377722064389343,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        53,
                    },
                    (double[]){
                        2.8011928354990574,
                        -0.23359195611402783,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        62,
                        63,
                    },
                    (double[]){
                        -3.3622446243837795e-18,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        63,
                        64,
                    },
                    (double[]){
                        -0.00030530715300446147,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        62,
                    },
                    (double[]){
                        1.1485744501587734,
                        -2.292734797535991,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        61,
                    },
                    (double[]){
                        0.9575446826239236,
                        -3.465587386063616,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        60,
                    },
                    (double[]){
                        2.4620925734551538,
                        1.5155400360600368,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        59,
                    },
                    (double[]){
                        2.2749539553785083,
                        -4.6863172020933455,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        58,
                    },
                    (double[]){
                        2.9606687428227003,
                        -2.373991477669906,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        57,
                    },
                    (double[]){
                        2.907692651101327,
                        -1.4369155154395827,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        56,
                    },
                    (double[]){
                        2.3395273213007,
                        -0.33748469818491644,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        55,
                    },
                    (double[]){
                        2.7089503985645282,
                        -0.6835159594911604,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        54,
                    },
                    (double[]){
                        2.5751254442413387,
                        -2.163338465066995,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        64,
                        65,
                    },
                    (double[]){
                        -0.001735199881523166,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        65,
                        66,
                    },
                    (double[]){
                        -0.004889240780821781,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        64,
                    },
                    (double[]){
                        1.95880434190825,
                        -0.1873123709786897,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        63,
                    },
                    (double[]){
                        2.0449703502619827,
                        0.36032937222223316,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        62,
                    },
                    (double[]){
                        2.375376357398631,
                        1.3769272052053392,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        61,
                    },
                    (double[]){
                        2.8710372113916476,
                        0.9632669138922791,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        60,
                    },
                    (double[]){
                        2.262586008727154,
                        -1.288370362859426,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        59,
                    },
                    (double[]){
                        2.3304866455186426,
                        -2.180128020851477,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        58,
                    },
                    (double[]){
                        2.9353609066189064,
                        -3.3001689532025207,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        57,
                    },
                    (double[]){
                        2.6569427995689936,
                        -0.2880230591158375,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        56,
                    },
                    (double[]){
                        2.8972242507940784,
                        0.6606327047745335,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        55,
                    },
                    (double[]){
                        2.637578306224484,
                        -0.1297496490111094,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        66,
                        67,
                    },
                    (double[]){
                        -0.0061687747756473685,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        67,
                        68,
                    },
                    (double[]){
                        -0.007921166010645917,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        66,
                    },
                    (double[]){
                        1.3952661967326057,
                        1.5430907982275288,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        65,
                    },
                    (double[]){
                        2.4586706200703428,
                        -4.410828153607344,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        64,
                    },
                    (double[]){
                        2.4913044734413945,
                        0.8713268735802138,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        63,
                    },
                    (double[]){
                        1.666532151939135,
                        1.565665605313919,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        62,
                    },
                    (double[]){
                        2.230091520874786,
                        -1.4649062073220245,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        61,
                    },
                    (double[]){
                        2.9230761214729517,
                        0.6183467120914266,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        60,
                    },
                    (double[]){
                        2.5800551498051942,
                        -1.8572280415164675,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        59,
                    },
                    (double[]){
                        2.882691716351001,
                        0.7527172167199372,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        58,
                    },
                    (double[]){
                        2.4450464342021716,
                        -3.6183762373801622,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        57,
                    },
                    (double[]){
                        2.817425709036258,
                        -1.1910839927076715,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        56,
                    },
                    (double[]){
                        2.775298602477821,
                        -3.2138698482015826,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        68,
                        69,
                    },
                    (double[]){
                        -0.011516021679808202,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        69,
                        70,
                    },
                    (double[]){
                        -0.025916562177373068,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        68,
                    },
                    (double[]){
                        1.6840267276395249,
                        1.3746712052149852,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        67,
                    },
                    (double[]){
                        1.8669601257467952,
                        -2.4444792677705824,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        66,
                    },
                    (double[]){
                        1.8301383984801725,
                        0.22910785102960962,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        65,
                    },
                    (double[]){
                        2.7311693460833824,
                        -3.6583006939931435,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        64,
                    },
                    (double[]){
                        2.1260114700692845,
                        1.4086044909515887,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        63,
                    },
                    (double[]){
                        1.8850080179093571,
                        -0.9650941623735478,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        62,
                    },
                    (double[]){
                        2.249214594410175,
                        -2.4803284947061406,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        61,
                    },
                    (double[]){
                        2.6155650341156544,
                        -2.205981649883237,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        60,
                    },
                    (double[]){
                        2.778968709622577,
                        -0.33626981631042474,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        59,
                    },
                    (double[]){
                        2.5186210453057445,
                        0.720163020649025,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        58,
                    },
                    (double[]){
                        2.388143234823213,
                        -3.4976489268925866,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        57,
                    },
                    (double[]){
                        2.527133442038302,
                        -1.33481245875564,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        70,
                        71,
                    },
                    (double[]){
                        -0.13138977063711743,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        71,
                        70,
                    },
                    (double[]){
                        2.9425523988971585,
                        1.244459690259606,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        69,
                    },
                    (double[]){
                        2.5977548969047333,
                        0.5116690534924846,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        68,
                    },
                    (double[]){
                        1.4687175815231555,
                        0.9195661617661721,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        67,
                    },
                    (double[]){
                        2.5966094707827123,
                        -3.841809837929219,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        66,
                    },
                    (double[]){
                        2.857606667943773,
                        0.2922107924779702,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        65,
                    },
                    (double[]){
                        2.217134679040776,
                        -4.384792148943332,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        64,
                    },
                    (double[]){
                        2.3606467022857913,
                        -3.317246580699604,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        63,
                    },
                    (double[]){
                        2.524675909472037,
                        -3.8871984509465953,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        62,
                    },
                    (double[]){
                        2.647370764930284,
                        -0.6606543668337115,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        61,
                    },
                    (double[]){
                        2.833740212280427,
                        -2.6981050568896148,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        60,
                    },
                    (double[]){
                        2.6071332045159634,
                        -0.030583533201665514,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        59,
                    },
                    (double[]){
                        3.0037194397026665,
                        -2.8932672727997018,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        58,
                    },
                    (double[]){
                        2.903305332662455,
                        -3.578931423157311,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        71,
                    },
                    (double[]){
                        2.0210641802042426,
                        -0.551123437114351,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        70,
                    },
                    (double[]){
                        2.087775029547971,
                        -2.7500858506231873,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        69,
                    },
                    (double[]){
                        2.1184565902501484,
                        -3.8693632019117348,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        68,
                    },
                    (double[]){
                        2.5805046941317165,
                        -0.5812836528828598,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        67,
                    },
                    (double[]){
                        2.650703706279322,
                        -0.44477955355608745,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        66,
                    },
                    (double[]){
                        2.056191870394667,
                        -1.9787869019209845,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        65,
                    },
                    (double[]){
                        2.590049569273978,
                        -2.591237663381828,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        64,
                    },
                    (double[]){
                        3.041007499237274,
                        -0.19363824945661934,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        63,
                    },
                    (double[]){
                        2.404272376757764,
                        1.0531277859521406,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        62,
                    },
                    (double[]){
                        2.649632527416618,
                        -3.0388033668880587,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        61,
                    },
                    (double[]){
                        2.9041620654312825,
                        -1.5416185594488774,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        60,
                    },
                    (double[]){
                        2.444510836099629,
                        -0.10942558713670758,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        71,
                    },
                    (double[]){
                        1.533542493279316,
                        -3.7479532954880965,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        70,
                    },
                    (double[]){
                        2.4726061902963212,
                        -1.3221128190269889,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        69,
                    },
                    (double[]){
                        2.8991394271685422,
                        0.967069086431434,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        68,
                    },
                    (double[]){
                        2.4012311762556884,
                        -4.395089074120078,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        67,
                    },
                    (double[]){
                        2.7777596954336636,
                        -2.3033070755442733,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        66,
                    },
                    (double[]){
                        2.583345204168922,
                        -0.4998945275083453,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        65,
                    },
                    (double[]){
                        2.6438460268628843,
                        -0.05393188300760898,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        64,
                    },
                    (double[]){
                        2.7943633732662674,
                        -2.4081597994340584,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        63,
                    },
                    (double[]){
                        2.974342923961377,
                        -4.4379877419307725,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        62,
                    },
                    (double[]){
                        2.926509629789757,
                        -4.452286722683915,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        61,
                    },
                    (double[]){
                        2.8580796594271427,
                        1.2730365118080633,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        71,
                    },
                    (double[]){
                        1.7038068755110596,
                        -0.6243514863436643,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        70,
                    },
                    (double[]){
                        2.004763129741312,
                        -1.9184020066944294,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        69,
                    },
                    (double[]){
                        2.5649194591236775,
                        -1.5833973286830174,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        68,
                    },
                    (double[]){
                        2.6684606183552666,
                        -2.912209056266535,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        67,
                    },
                    (double[]){
                        2.5713681923008185,
                        1.1975507754839918,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        66,
                    },
                    (double[]){
                        2.9379639216410274,
                        -0.720873854234127,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        65,
                    },
                    (double[]){
                        2.477562700881187,
                        -1.1393338971398763,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        64,
                    },
                    (double[]){
                        2.6297181868801864,
                        1.2235048982973615,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        63,
                    },
                    (double[]){
                        3.0058250746759203,
                        -3.2311610620634603,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        62,
                    },
                    (double[]){
                        2.684126721032168,
                        -4.591300612116007,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        71,
                    },
                    (double[]){
                        1.7536327706080703,
                        0.8936871601852054,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        70,
                    },
                    (double[]){
                        2.6033331883323743,
                        -1.3247631881996476,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        69,
                    },
                    (double[]){
                        2.632336287334508,
                        1.3306789845174936,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        68,
                    },
                    (double[]){
                        2.521098055694825,
                        1.4611841682738578,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        67,
                    },
                    (double[]){
                        2.7066268950849746,
                        -0.02824881381915989,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        66,
                    },
                    (double[]){
                        2.8607366954880113,
                        -2.576894264373272,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        65,
                    },
                    (double[]){
                        2.8199273789059625,
                        0.9147556964154977,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        64,
                    },
                    (double[]){
                        2.787286454096322,
                        -1.7507957281446211,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        63,
                    },
                    (double[]){
                        2.7477718965437687,
                        -3.123114100474603,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        71,
                    },
                    (double[]){
                        1.6908574051152372,
                        -0.32626491071549113,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        70,
                    },
                    (double[]){
                        1.596667865950821,
                        -2.6555932404886655,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        69,
                    },
                    (double[]){
                        2.8709162748558796,
                        -3.7857927744684448,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        68,
                    },
                    (double[]){
                        2.7781822561835328,
                        -3.8457711354452107,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        67,
                    },
                    (double[]){
                        2.4803897860056856,
                        -2.4961210243174516,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        66,
                    },
                    (double[]){
                        2.4267145925396347,
                        -0.10408971690399205,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        65,
                    },
                    (double[]){
                        2.571553195663667,
                        -0.2082231811337074,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        64,
                    },
                    (double[]){
                        2.5129411776160424,
                        -4.111576682887643,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        71,
                    },
                    (double[]){
                        2.972440719308561,
                        -0.574456083783246,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        70,
                    },
                    (double[]){
                        2.723537763406409,
                        -0.19587625915704798,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        69,
                    },
                    (double[]){
                        2.3587844421592323,
                        1.1362268380479352,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        68,
                    },
                    (double[]){
                        1.9984493337912554,
                        -3.390226961994027,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        67,
                    },
                    (double[]){
                        2.3529877699559005,
                        1.2389383797299987,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        66,
                    },
                    (double[]){
                        2.7713007362856614,
                        0.8867841628862325,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        65,
                    },
                    (double[]){
                        2.6430312781134875,
                        0.47757632191837684,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        71,
                    },
                    (double[]){
                        1.8752961191630328,
                        0.5980980375219334,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        70,
                    },
                    (double[]){
                        0.8897649234049255,
                        -0.5290590326528712,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        69,
                    },
                    (double[]){
                        2.9510331485851946,
                        -2.41976204643964,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        68,
                    },
                    (double[]){
                        2.8228569752854598,
                        1.2793405799811803,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        67,
                    },
                    (double[]){
                        3.087747497059217,
                        -3.840205915315935,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        66,
                    },
                    (double[]){
                        2.144689330430432,
                        -2.8452421880548897,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        71,
                    },
                    (double[]){
                        1.7643645831398382,
                        -3.86609532141712,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        70,
                    },
                    (double[]){
                        2.3840718968397203,
                        -2.5821911648319045,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        69,
                    },
                    (double[]){
                        2.7067900446961133,
                        -3.8126043823983697,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        68,
                    },
                    (double[]){
                        2.056176729133506,
                        1.42320848780381,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        67,
                    },
                    (double[]){
                        2.9051430380636383,
                        -3.241125849043073,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        71,
                    },
                    (double[]){
                        2.566850714915489,
                        0.011836547825281984,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        70,
                    },
                    (double[]){
                        2.240855013816098,
                        -2.533996036884421,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        69,
                    },
                    (double[]){
                        2.0044155058794657,
                        -2.5790305954051247,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        68,
                    },
                    (double[]){
                        2.4144076119766806,
                        -3.8792820864595945,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        71,
                    },
                    (double[]){
                        1.9755719034098922,
                        -2.737459089474252,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        70,
                    },
                    (double[]){
                        2.1626070452317525,
                        1.0525054273598533,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        69,
                    },
                    (double[]){
                        2.0784423525438185,
                        -2.122379661305918,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        71,
                    },
                    (double[]){
                        2.9033659353537726,
                        1.1136882349587318,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        70,
                    },
                    (double[]){
                        2.2745327725624063,
                        1.1230296871601921,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        71,
                    },
                    (double[]){
                        1.9214687887805006,
                        0.24329186720151852,
                    });
    qk_circuit_gate(qc, QkGate_CPhase,
                    (uint32_t[]){
                        8,
                        44,
                    },
                    (double[]){
                        -0.00014021408597489102,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        2.5326753771986494,
                        -0.4223071786852588,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        2.879342716624786,
                        -0.4525440721328764,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        2.4893168602560802,
                        -0.22800979792397813,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        2.803681647999335,
                        -2.958594288527055,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        2.4091658522597847,
                        -3.4578994654340747,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        2.7247395605607543,
                        -4.4872328211397186,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        2.8790070632674674,
                        -4.221946272798072,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        37,
                    },
                    (double[]){
                        2.678508671594563,
                        -2.348064168469377,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        36,
                    },
                    (double[]){
                        2.4619775459668274,
                        0.19165231780468384,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        2.7936798380851595,
                        -4.23392723716433,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        2.3922589494810262,
                        -1.3652511343822331,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        2.3991896375827046,
                        -4.683504789110083,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        2.199344005041305,
                        -1.6791448997377585,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        2.57117402467766,
                        -3.504601857941653,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        2.847287476443901,
                        0.6011701969170322,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        2.603429551783166,
                        -0.48290440957584213,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        2.63279092839864,
                        -2.3865285553256035,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        37,
                    },
                    (double[]){
                        2.879275061845869,
                        -0.17404427746497775,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        36,
                    },
                    (double[]){
                        2.120542109271803,
                        -1.4947542893601702,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        2.6519811060877165,
                        -0.9658115691657598,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        2.8753609815971477,
                        -1.7984089279418476,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        2.4202445206466274,
                        1.0017712500167306,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        2.944059947693303,
                        0.7283304960167793,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        2.7551883002864996,
                        -2.5882750306735023,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        3.0094104970386892,
                        1.4907374587246323,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        3.086549426825471,
                        0.4220545007719154,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        2.9444815992992375,
                        1.2359852388580324,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        2.5104193716296836,
                        -4.303051616875107,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        37,
                    },
                    (double[]){
                        2.3968847157699944,
                        -3.3386328118460495,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        36,
                    },
                    (double[]){
                        1.95093187506634,
                        -1.9147602123400869,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        3.093006393626582,
                        -3.191301868458355,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        2.777936028078684,
                        -0.44455726716064414,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        3.0205789952649815,
                        -0.42263728425826574,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        2.804186993591348,
                        1.1975074021372731,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        2.7190531376207785,
                        1.0460291029219175,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        2.7179106092677605,
                        -4.087186457313529,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        3.0372038930636243,
                        -2.5934167083669477,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        2.673758253812591,
                        0.10130316311107568,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        2.84329345981634,
                        -2.843029777702397,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        2.493270802674935,
                        1.1248411735464638,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        37,
                    },
                    (double[]){
                        2.3167212237591595,
                        -1.6147047410553894,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        36,
                    },
                    (double[]){
                        0.9583114747465359,
                        -4.310385413718229,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        2.9942941609022853,
                        -0.24694274971016683,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        3.033612753011489,
                        -1.6150719206515722,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        2.757316449366157,
                        1.477259728653645,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        2.6397691336811215,
                        -3.880386032069793,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        2.370199846521615,
                        -0.21421050705225153,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        2.6669272926480025,
                        0.624934571152719,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        2.9339700837996943,
                        0.17700520201339542,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        2.846449856661585,
                        1.0815920567010489,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        2.8046197137302755,
                        -1.3490917452134548,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        2.6146411037819903,
                        -3.9818016740519124,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        2.610687839494691,
                        -3.597879231066967,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        37,
                    },
                    (double[]){
                        2.655443053079464,
                        -2.634356498670199,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        36,
                    },
                    (double[]){
                        1.0663965248189058,
                        -1.890975964370857,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        2.6135793105238347,
                        -1.9075214022772928,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        2.9449184396753116,
                        -2.745064125788979,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        2.9000861530290565,
                        0.6934292985045118,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        2.886364069210408,
                        1.5161538060701063,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        2.7189735902410175,
                        -3.322922687859119,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        2.993142198152061,
                        1.027383823218516,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        2.649560570512981,
                        -0.20231074349886113,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        2.6983275713553487,
                        -0.08326505010805052,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        3.009212840046099,
                        -1.9284748249314345,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        2.930370487046188,
                        -1.1775214901431144,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        2.2060630667073586,
                        -1.9603575639466397,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        2.4347375029293303,
                        -0.5875334950068896,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        37,
                    },
                    (double[]){
                        2.3913134159015677,
                        0.32230026788627586,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        36,
                    },
                    (double[]){
                        2.4258226584048357,
                        1.5444207955999585,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        3.0372706808401624,
                        1.2253641838201594,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        2.790489413242308,
                        -1.4998597053586074,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        2.7311642667418616,
                        -3.865302497986964,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        2.9217276294846224,
                        -2.470286452312313,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        3.0047004495328897,
                        -0.5829819452340257,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        2.895363839617158,
                        -2.245386614211008,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        2.58729503327512,
                        0.36958432833200683,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        2.8202223681062213,
                        -2.032713052950255,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        2.8065254651438583,
                        1.527407044185673,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        2.545027037440552,
                        -2.1144741209586835,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        2.6225831314470476,
                        -3.8472971764887363,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        2.6088107706431525,
                        -1.5818996221668449,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        2.307554627233103,
                        -3.0176692912733563,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        37,
                    },
                    (double[]){
                        2.0305341744077303,
                        -1.4105888421532455,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        36,
                    },
                    (double[]){
                        2.0227471212280315,
                        1.3684084591698684,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        51,
                    },
                    (double[]){
                        2.5639434559624834,
                        -1.1476109048057501,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        2.915695893804007,
                        -2.0096507734874045,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        2.9541294333315413,
                        0.020648996036576772,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        2.863569587551981,
                        1.2305538277218941,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        2.743258840206361,
                        -1.0581922621459228,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        2.8008278626687657,
                        -4.612904224724799,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        2.7450385171435814,
                        -1.224847745416179,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        2.823919288843189,
                        -4.383023815954063,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        3.017578119525451,
                        -1.7061214882078568,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        2.712967334243969,
                        -4.511822301400015,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        2.4566768973066684,
                        -3.7542518567625516,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        2.424542253526856,
                        -2.4128990447024385,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        2.9121225698037927,
                        -1.764033741256254,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        2.5725177335907885,
                        -0.4434307989825108,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        37,
                    },
                    (double[]){
                        1.9093734253148449,
                        -1.0177845746209546,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        36,
                    },
                    (double[]){
                        2.172445878632686,
                        -0.927513722982384,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        52,
                    },
                    (double[]){
                        2.8288982060198475,
                        1.501040820991693,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        51,
                    },
                    (double[]){
                        2.2324382297414878,
                        -4.072104540538202,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        2.511673024286581,
                        -3.9058847304825663,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        2.698172240482159,
                        -0.28720351034326574,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        2.7991499199330794,
                        -3.3156598121276977,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        2.474632586801765,
                        -4.457555665067929,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        2.9442014677102697,
                        -3.8669061579100363,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        2.613571864571477,
                        -4.517868017833777,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        2.907625344232878,
                        0.1795712708003292,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        3.0466492537345533,
                        1.2915453210130425,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        2.5504109763848843,
                        -3.7776089068259586,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        2.6484441557498957,
                        -1.8682289912243395,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        2.0149953832403265,
                        -0.5237207454390396,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        2.2945026487329057,
                        -4.596709915381738,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        2.419560190530837,
                        1.2325490983646956,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        37,
                    },
                    (double[]){
                        2.4295597977512244,
                        -0.8152910106979927,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        36,
                    },
                    (double[]){
                        1.8350276077342873,
                        -2.9199196287314013,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        53,
                    },
                    (double[]){
                        2.776083247497897,
                        1.3414436419646063,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        52,
                    },
                    (double[]){
                        3.001654906200765,
                        -2.975087618010079,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        51,
                    },
                    (double[]){
                        2.6858304319380224,
                        0.37293767582821946,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        3.0044721432412955,
                        -3.7581945681911413,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        2.4095717733740605,
                        -3.9535004619322986,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        2.816312690085415,
                        1.046853238568267,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        2.317675936208363,
                        0.05078098373074735,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        2.869906185659958,
                        -3.5548498695814805,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        2.9365212943109804,
                        -3.259640175969666,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        2.519210737722506,
                        -3.004716297204361,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        2.739784569227518,
                        -0.07659879917098755,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        2.6443629603436487,
                        -0.4569305441184859,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        3.0224054656754467,
                        -0.5941459575267745,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        2.5842394765754255,
                        -2.7438035170105337,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        2.9038047795522215,
                        1.1495651486333536,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        1.8095336894868959,
                        -4.035625150226755,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        37,
                    },
                    (double[]){
                        2.808040235920636,
                        -1.065016823876206,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        36,
                    },
                    (double[]){
                        2.0457699832487504,
                        0.47143886669291435,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        54,
                    },
                    (double[]){
                        2.8554729815800215,
                        -2.1297622278845703,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        53,
                    },
                    (double[]){
                        2.678329118768442,
                        -2.7132267950870546,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        52,
                    },
                    (double[]){
                        2.748805365566814,
                        -3.8933020928839075,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        51,
                    },
                    (double[]){
                        3.054338009536866,
                        0.5056686186254127,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        2.7163620781150195,
                        -3.077326479357918,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        2.920714563965329,
                        -1.2617499631587137,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        2.931109968877068,
                        -2.7075440090493554,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        2.9301659039597925,
                        -4.683812694573707,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        2.824774588550247,
                        -1.46046255057644,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        2.8900807950164786,
                        -1.9085493235914246,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        2.8684479127776594,
                        -4.586095585027394,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        2.5938246867490076,
                        0.41673895204009637,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        3.0024239200004366,
                        -3.8863564484682773,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        2.7851758749226265,
                        1.5039647590748633,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        2.883230332260518,
                        1.3441897894439214,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        2.747392852499555,
                        -4.185439133911553,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        1.8698921314341648,
                        0.8990260073567118,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        37,
                    },
                    (double[]){
                        1.3824196931257122,
                        -0.4427937199631886,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        36,
                    },
                    (double[]){
                        1.85697837848801,
                        -3.823570358826113,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        55,
                    },
                    (double[]){
                        2.8854206084721232,
                        -2.8866215603154,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        54,
                    },
                    (double[]){
                        2.5858053398300305,
                        -2.0771787182425085,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        53,
                    },
                    (double[]){
                        2.7940270634369977,
                        1.5140615037443381,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        52,
                    },
                    (double[]){
                        2.9464997073876997,
                        -0.02411759135282865,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        51,
                    },
                    (double[]){
                        2.8593638706218587,
                        -4.064281953518923,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        3.0361683068334364,
                        -0.23815918646251522,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        2.7193724290264054,
                        -4.053425039917732,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        2.743630036564101,
                        -2.817849408379219,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        3.030764944802419,
                        -0.46912609670142125,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        2.5170275273479783,
                        1.312869484697961,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        2.745868719138243,
                        -0.4235232425328843,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        1.9990613767462997,
                        -4.1226431747204995,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        3.0073226349639897,
                        -3.6147051653623725,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        2.7993985141692668,
                        -3.140748857459534,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        2.8860492839572376,
                        1.3576033662075035,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        2.5341231986689774,
                        1.4383962300349662,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        3.0419256105812273,
                        -2.951214608107607,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        2.436316500931743,
                        -1.7962446160248091,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        37,
                    },
                    (double[]){
                        1.834055095870482,
                        -3.3237354482391126,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        36,
                    },
                    (double[]){
                        0.9152513179553405,
                        -2.459334355652267,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        56,
                    },
                    (double[]){
                        2.56055729123577,
                        -4.68394911075948,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        55,
                    },
                    (double[]){
                        2.307964013057533,
                        0.5161854834642918,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        54,
                    },
                    (double[]){
                        3.0529263971988048,
                        -4.516384849733665,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        53,
                    },
                    (double[]){
                        2.542948625588145,
                        0.5016269515184759,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        52,
                    },
                    (double[]){
                        2.774096066627165,
                        -2.553314345413077,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        51,
                    },
                    (double[]){
                        2.9810347121926317,
                        1.3117014425376952,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        2.4284977478770533,
                        -4.677668636681988,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        2.7589322123480766,
                        -1.1848321448026355,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        2.485728613395424,
                        -4.16338891214464,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        2.414683370699701,
                        -3.942444054292303,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        2.588346867143977,
                        0.5419704825488645,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        2.5997039021156736,
                        -3.975304665429338,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        2.620273588717665,
                        -3.7343461864773815,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        2.475404747487059,
                        -2.101628188777282,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        3.0428582147740433,
                        -4.631024430774277,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        2.781607979175291,
                        -3.5477676198112276,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        1.8477817821826783,
                        0.38966439544398424,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        3.1007456041872024,
                        -0.7322881466876723,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        1.8030745898778033,
                        -3.7652110506944445,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        37,
                    },
                    (double[]){
                        2.9238415834030613,
                        -0.9760751245139779,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        36,
                    },
                    (double[]){
                        1.1379270381117592,
                        -0.5261716958513794,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        57,
                    },
                    (double[]){
                        2.613635714662012,
                        1.0578293928445563,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        56,
                    },
                    (double[]){
                        2.8949745624652063,
                        0.010906733766737897,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        55,
                    },
                    (double[]){
                        2.967791085194453,
                        -0.3569945841166511,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        54,
                    },
                    (double[]){
                        3.042663996981131,
                        -1.2284373726074378,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        53,
                    },
                    (double[]){
                        2.9167979313849544,
                        -4.677841901332673,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        52,
                    },
                    (double[]){
                        2.517292658846209,
                        0.648188954683421,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        51,
                    },
                    (double[]){
                        3.0875221852263897,
                        0.868734712094847,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        50,
                    },
                    (double[]){
                        2.7677216482597005,
                        -0.857885283723141,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        49,
                    },
                    (double[]){
                        3.062161417988011,
                        -4.151710096799841,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        48,
                    },
                    (double[]){
                        2.7354536701198144,
                        -3.4971923231155655,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        47,
                    },
                    (double[]){
                        2.4594885652629905,
                        -2.068564039159127,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        46,
                    },
                    (double[]){
                        2.58468400111932,
                        -3.5977420227743586,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        45,
                    },
                    (double[]){
                        2.4926165091054364,
                        -2.037060050085576,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        44,
                    },
                    (double[]){
                        2.7813304267248626,
                        1.1745383479770752,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        43,
                    },
                    (double[]){
                        2.717916531749616,
                        -1.927132542919367,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        42,
                    },
                    (double[]){
                        2.703478864258205,
                        -1.0605409461818711,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        41,
                    },
                    (double[]){
                        2.618102623098778,
                        1.5246585168711735,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        40,
                    },
                    (double[]){
                        2.5978355600846195,
                        -1.752850679728389,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        39,
                    },
                    (double[]){
                        2.7072532555729305,
                        -0.2703049957542234,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        38,
                    },
                    (double[]){
                        2.640559198072321,
                        1.2426914634545239,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        37,
                    },
                    (double[]){
                        2.0930630358400233,
                        -4.533183474048242,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        36,
                    },
                    (double[]){
                        0.31682329698762796,
                        -2.4919415397021454,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        36,
                    },
                    (double[]){
                        1.2773736996672451,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        59,
                    },
                    (double[]){
                        2.6996713148676816,
                        1.368719083807946,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        58,
                    },
                    (double[]){
                        2.7096494726210216,
                        0.6695373231143944,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        57,
                    },
                    (double[]){
                        2.7887057636942,
                        -2.7240395330940927,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        56,
                    },
                    (double[]){
                        2.9099076374697077,
                        0.7358535687578036,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        55,
                    },
                    (double[]){
                        2.8958715655881555,
                        -3.825238353187724,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        54,
                    },
                    (double[]){
                        2.8313422693182453,
                        -2.682499764846015,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        53,
                    },
                    (double[]){
                        2.749467203365316,
                        -0.6991642603202927,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        52,
                    },
                    (double[]){
                        2.8656263217101317,
                        -2.585230027503467,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        51,
                    },
                    (double[]){
                        2.6345426644926073,
                        -2.5625199403259784,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        50,
                    },
                    (double[]){
                        2.851079448807529,
                        1.2654363027034496,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        49,
                    },
                    (double[]){
                        2.8576632893378977,
                        -4.704999732684566,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        48,
                    },
                    (double[]){
                        2.680244274781544,
                        -0.9191953313400474,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        47,
                    },
                    (double[]){
                        2.7533516238636953,
                        -0.9491026308244892,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        46,
                    },
                    (double[]){
                        2.7921474004478477,
                        -3.107713396346086,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        45,
                    },
                    (double[]){
                        2.5202832434520346,
                        -2.59501028597785,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        44,
                    },
                    (double[]){
                        2.7848500793290007,
                        0.03344164557895213,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        43,
                    },
                    (double[]){
                        2.691710610681685,
                        -2.381590646391839,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        42,
                    },
                    (double[]){
                        2.514409968774285,
                        -2.026534545349116,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        41,
                    },
                    (double[]){
                        2.9801106538067295,
                        -3.9628352870435024,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        40,
                    },
                    (double[]){
                        2.61778895458567,
                        -2.01933940769691,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        38,
                        39,
                    },
                    (double[]){
                        1.9381688030086426,
                        0.5643524965780573,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        37,
                        38,
                    },
                    (double[]){
                        2.4234077043916438,
                        1.5679908851799338,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        60,
                    },
                    (double[]){
                        2.656413457296041,
                        1.4414963822356137,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        59,
                    },
                    (double[]){
                        2.9369850060196536,
                        -1.2740931299119762,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        58,
                    },
                    (double[]){
                        2.9414852478555567,
                        -3.3777421763097326,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        57,
                    },
                    (double[]){
                        2.719739894778777,
                        -0.31213556032272405,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        56,
                    },
                    (double[]){
                        2.5979849642649206,
                        -2.707063442866503,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        55,
                    },
                    (double[]){
                        2.4073651680443042,
                        0.5633458501447084,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        54,
                    },
                    (double[]){
                        2.6663390772332187,
                        -1.5133740543649359,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        53,
                    },
                    (double[]){
                        2.8386818136081517,
                        -3.6923848663875374,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        52,
                    },
                    (double[]){
                        2.8817666489302756,
                        1.4941746366693445,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        51,
                    },
                    (double[]){
                        2.9125745000996197,
                        -2.5342375911219643,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        50,
                    },
                    (double[]){
                        2.8262037107185516,
                        -3.3340824194591336,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        49,
                    },
                    (double[]){
                        2.9486291911372153,
                        -2.2197023314620514,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        48,
                    },
                    (double[]){
                        2.4798692315333963,
                        -1.2877357768685183,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        47,
                    },
                    (double[]){
                        2.830155913179915,
                        0.6292333358587832,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        46,
                    },
                    (double[]){
                        2.4834145183925074,
                        -3.38207369739457,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        45,
                    },
                    (double[]){
                        2.211137114526412,
                        -0.793815318042057,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        44,
                    },
                    (double[]){
                        2.453102195575799,
                        -3.903876956520319,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        43,
                    },
                    (double[]){
                        2.2172038902859943,
                        -0.9834826472683712,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        42,
                    },
                    (double[]){
                        2.5977771014683078,
                        -3.339652222642604,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        40,
                        41,
                    },
                    (double[]){
                        1.3447948547353714,
                        0.15478764923214405,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        39,
                        40,
                    },
                    (double[]){
                        2.5013939760235946,
                        -4.267135210767637,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        39,
                    },
                    (double[]){
                        -0.4537570254817901,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        61,
                    },
                    (double[]){
                        2.790159216334415,
                        -1.7623776587436342,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        60,
                    },
                    (double[]){
                        2.8651056318325803,
                        -0.4937387658851402,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        59,
                    },
                    (double[]){
                        2.9265047714823758,
                        1.548577386983776,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        58,
                    },
                    (double[]){
                        2.8577697455409847,
                        -1.742296565948419,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        57,
                    },
                    (double[]){
                        2.428212976428276,
                        -4.607018589701775,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        56,
                    },
                    (double[]){
                        2.6141117307553565,
                        -0.9244345742049852,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        55,
                    },
                    (double[]){
                        2.7083216942087933,
                        -1.008546740183603,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        54,
                    },
                    (double[]){
                        2.7663143390426415,
                        -3.732807910158822,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        53,
                    },
                    (double[]){
                        2.8419167146962794,
                        -1.7044243230395955,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        52,
                    },
                    (double[]){
                        2.7037375288857315,
                        -0.5021187856334819,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        51,
                    },
                    (double[]){
                        2.6740553288034468,
                        0.7439216608233821,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        50,
                    },
                    (double[]){
                        2.707415219650358,
                        0.3603338644824263,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        49,
                    },
                    (double[]){
                        2.6639103402819426,
                        0.9878483294958627,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        48,
                    },
                    (double[]){
                        2.951618302301949,
                        1.1209587172812956,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        47,
                    },
                    (double[]){
                        2.856782488922934,
                        -2.4185835197004923,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        46,
                    },
                    (double[]){
                        2.0943581648264566,
                        -0.36872052088278795,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        45,
                    },
                    (double[]){
                        2.566546691458132,
                        -0.7222944905933069,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        44,
                    },
                    (double[]){
                        1.6053584531614993,
                        -0.14949961503787002,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        42,
                        43,
                    },
                    (double[]){
                        2.9786587773229534,
                        -2.8371063954572575,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        41,
                        42,
                    },
                    (double[]){
                        1.9497920514489435,
                        1.366786495526918,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        62,
                    },
                    (double[]){
                        2.482044775685297,
                        -4.026910927117437,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        61,
                    },
                    (double[]){
                        3.0164526539292336,
                        0.19344682089943088,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        60,
                    },
                    (double[]){
                        2.931020232420629,
                        0.8812303766050569,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        59,
                    },
                    (double[]){
                        2.589780817468795,
                        -3.885679153180544,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        58,
                    },
                    (double[]){
                        2.995644869786327,
                        -3.336380008998653,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        57,
                    },
                    (double[]){
                        3.043378864938286,
                        -1.8904581676720453,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        56,
                    },
                    (double[]){
                        2.7549108182199697,
                        -1.6101309210564447,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        55,
                    },
                    (double[]){
                        2.8216712958744217,
                        -2.337468754379637,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        54,
                    },
                    (double[]){
                        2.846004282959235,
                        -0.6557930292154726,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        53,
                    },
                    (double[]){
                        2.6411251920815197,
                        -0.28839221593751785,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        52,
                    },
                    (double[]){
                        2.6536667819373836,
                        -2.704824712278417,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        51,
                    },
                    (double[]){
                        3.0050337077516374,
                        -3.8403376353020797,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        50,
                    },
                    (double[]){
                        3.017211334920625,
                        -1.4326017618066866,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        49,
                    },
                    (double[]){
                        2.477949351237913,
                        0.015328422591699864,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        48,
                    },
                    (double[]){
                        2.699033703759462,
                        -0.28034190495011657,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        47,
                    },
                    (double[]){
                        2.5780455790590038,
                        0.767086092322542,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        46,
                    },
                    (double[]){
                        3.0051008161739956,
                        -4.667263252762956,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        44,
                        45,
                    },
                    (double[]){
                        2.918850501259475,
                        -1.8905119389698948,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        43,
                        44,
                    },
                    (double[]){
                        2.2910638814623088,
                        -0.14445737979806794,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        63,
                    },
                    (double[]){
                        2.8366999476608026,
                        -2.3245631113340015,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        62,
                    },
                    (double[]){
                        3.0113716935235577,
                        -3.6783014277973844,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        61,
                    },
                    (double[]){
                        2.7999050613630097,
                        0.7178794463320433,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        60,
                    },
                    (double[]){
                        2.5899808338179473,
                        -0.22588256812947982,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        59,
                    },
                    (double[]){
                        2.8308216524283827,
                        -3.416960856178253,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        58,
                    },
                    (double[]){
                        2.720589946945526,
                        -4.134401505337911,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        57,
                    },
                    (double[]){
                        2.708431532260987,
                        0.1744292287149929,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        56,
                    },
                    (double[]){
                        2.784005714402471,
                        0.1295558595107833,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        55,
                    },
                    (double[]){
                        2.5677852886433863,
                        -3.5620616274777173,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        54,
                    },
                    (double[]){
                        2.968137940933438,
                        -4.601482305488428,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        53,
                    },
                    (double[]){
                        2.2820058195353012,
                        -0.2774181368443447,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        52,
                    },
                    (double[]){
                        2.8618301481034925,
                        -3.731494687822249,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        51,
                    },
                    (double[]){
                        2.5298102667879414,
                        -2.972312847080187,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        50,
                    },
                    (double[]){
                        2.2464886534905975,
                        -0.13008711868035938,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        49,
                    },
                    (double[]){
                        2.5960226542831277,
                        -1.584165592052527,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        48,
                    },
                    (double[]){
                        2.7573366880722663,
                        -2.6784726300871524,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        46,
                        47,
                    },
                    (double[]){
                        2.2569670051611777,
                        -1.6737072212867874,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        45,
                        46,
                    },
                    (double[]){
                        0.8695858645683318,
                        -3.975521650689593,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        64,
                    },
                    (double[]){
                        2.9810899738033987,
                        -2.5522542560150634,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        63,
                    },
                    (double[]){
                        2.3563237812351794,
                        -2.583655719545386,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        62,
                    },
                    (double[]){
                        2.8420795312416636,
                        -1.6514976924642937,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        61,
                    },
                    (double[]){
                        2.8160557474184134,
                        -0.8028452990394259,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        60,
                    },
                    (double[]){
                        2.9720035194730485,
                        0.6325693310707039,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        59,
                    },
                    (double[]){
                        2.579987427558504,
                        -0.8508338287522145,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        58,
                    },
                    (double[]){
                        2.5590078715506346,
                        0.6478487518054292,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        57,
                    },
                    (double[]){
                        2.911592729628225,
                        0.6966293655419813,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        56,
                    },
                    (double[]){
                        2.727521116259641,
                        0.40520856976312114,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        55,
                    },
                    (double[]){
                        1.967410702001562,
                        -1.6080762518316918,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        54,
                    },
                    (double[]){
                        2.3652924001986406,
                        -2.4146900012797916,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        53,
                    },
                    (double[]){
                        2.7005446224370018,
                        -1.8422282409808455,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        52,
                    },
                    (double[]){
                        2.9625368121020044,
                        -4.482259561729266,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        51,
                    },
                    (double[]){
                        2.790836153509008,
                        -2.3965127076766657,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        50,
                    },
                    (double[]){
                        2.978302131339481,
                        -2.0624652997656865,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        48,
                        49,
                    },
                    (double[]){
                        2.6692297612817732,
                        0.49309177298598916,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        47,
                        48,
                    },
                    (double[]){
                        1.45634970258828,
                        1.0945920219840315,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        47,
                    },
                    (double[]){
                        -2.3519311962914298,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        65,
                    },
                    (double[]){
                        2.9072225034862678,
                        0.9903412450993394,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        64,
                    },
                    (double[]){
                        2.6698657880840844,
                        -0.5307490335321754,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        63,
                    },
                    (double[]){
                        2.758670357005995,
                        -0.9579283196751032,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        62,
                    },
                    (double[]){
                        2.6877735012378654,
                        -2.9093344351239763,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        61,
                    },
                    (double[]){
                        2.71030799604598,
                        -3.587728453138113,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        60,
                    },
                    (double[]){
                        2.7520613861889345,
                        -0.21810887780280486,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        59,
                    },
                    (double[]){
                        2.9262051344158437,
                        -0.7755886161512453,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        58,
                    },
                    (double[]){
                        2.775329628642072,
                        -0.15109849431860534,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        57,
                    },
                    (double[]){
                        2.6094720237432494,
                        -3.981822332872958,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        56,
                    },
                    (double[]){
                        2.8648036946046274,
                        0.08045252085793386,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        55,
                    },
                    (double[]){
                        2.7424682843972565,
                        0.8737800593538672,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        54,
                    },
                    (double[]){
                        2.320508172421448,
                        -0.06422034117130604,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        53,
                    },
                    (double[]){
                        2.979929021266038,
                        -4.0925521973167776,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        52,
                    },
                    (double[]){
                        2.433255702121207,
                        -4.131258303203252,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        50,
                        51,
                    },
                    (double[]){
                        2.0915685637206947,
                        -1.3011102353420392,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        49,
                        50,
                    },
                    (double[]){
                        1.9406612399783745,
                        -4.5272000249726805,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        49,
                    },
                    (double[]){
                        1.2257299215679558,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        66,
                    },
                    (double[]){
                        2.1915138520027018,
                        0.3667812575679914,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        65,
                    },
                    (double[]){
                        2.4025083574976245,
                        0.8856199847285269,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        64,
                    },
                    (double[]){
                        2.720459240903196,
                        0.3814099489566223,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        63,
                    },
                    (double[]){
                        2.485531331098659,
                        -3.92893770554438,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        62,
                    },
                    (double[]){
                        2.5295707377000927,
                        0.5193363188771558,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        61,
                    },
                    (double[]){
                        2.796347014267797,
                        -3.5314172117982996,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        60,
                    },
                    (double[]){
                        2.6769851222602288,
                        -4.212865360649664,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        59,
                    },
                    (double[]){
                        2.7032476978451534,
                        -2.33827893309296,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        58,
                    },
                    (double[]){
                        2.5476073014626874,
                        1.0082697712244624,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        57,
                    },
                    (double[]){
                        2.543465779896254,
                        0.5295731844759368,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        56,
                    },
                    (double[]){
                        2.1929262497787625,
                        -2.210765004224075,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        55,
                    },
                    (double[]){
                        2.8887084587563026,
                        -4.451788061793018,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        54,
                    },
                    (double[]){
                        2.20474139038771,
                        -4.003212313992163,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        52,
                        53,
                    },
                    (double[]){
                        2.898774292544521,
                        -0.9486275774850728,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        51,
                        52,
                    },
                    (double[]){
                        1.3140783372057763,
                        -1.1765341254204567,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        51,
                    },
                    (double[]){
                        0.8915145183315308,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        52,
                    },
                    (double[]){
                        -2.1463397672114373,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        67,
                    },
                    (double[]){
                        2.5110896423999205,
                        0.37162234942138794,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        66,
                    },
                    (double[]){
                        2.5236797873191374,
                        -1.860523407515669,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        65,
                    },
                    (double[]){
                        2.5708976998950135,
                        -0.4877927363512107,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        64,
                    },
                    (double[]){
                        2.816373606410593,
                        1.0688742893989622,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        63,
                    },
                    (double[]){
                        2.4293963019809577,
                        -1.3891203039279008,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        62,
                    },
                    (double[]){
                        2.671194891555819,
                        -0.4307430478604888,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        61,
                    },
                    (double[]){
                        2.4497774635599256,
                        -4.3513308042320835,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        60,
                    },
                    (double[]){
                        2.9457108778383483,
                        -2.8260185403413436,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        59,
                    },
                    (double[]){
                        2.9590613870838105,
                        -2.504509988456335,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        58,
                    },
                    (double[]){
                        2.8538857982139048,
                        -1.037800832610594,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        57,
                    },
                    (double[]){
                        2.1614964837533006,
                        -2.44514085918127,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        56,
                    },
                    (double[]){
                        2.3315317568616507,
                        -0.6572578709569132,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        54,
                        55,
                    },
                    (double[]){
                        2.760088116520873,
                        -0.9631507107937185,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        53,
                        54,
                    },
                    (double[]){
                        0.7709075291363177,
                        -4.107327589281249,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        53,
                    },
                    (double[]){
                        2.51539187335296,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        68,
                    },
                    (double[]){
                        2.3289778355106927,
                        -3.550440306662404,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        67,
                    },
                    (double[]){
                        2.988633215991043,
                        -3.390254936871692,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        66,
                    },
                    (double[]){
                        2.6905258493118653,
                        0.8702980010646559,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        65,
                    },
                    (double[]){
                        2.6844318584406115,
                        -4.564675106423861,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        64,
                    },
                    (double[]){
                        2.776914025353056,
                        -1.5302459471957932,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        63,
                    },
                    (double[]){
                        2.6689566868670447,
                        0.3473282981908383,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        62,
                    },
                    (double[]){
                        2.5003097674693895,
                        -4.457119708805836,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        61,
                    },
                    (double[]){
                        2.871866160447497,
                        -0.7345078552580117,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        60,
                    },
                    (double[]){
                        2.773333027155682,
                        0.35179789177437004,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        59,
                    },
                    (double[]){
                        2.4096198462555485,
                        -3.8630961391100334,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        58,
                    },
                    (double[]){
                        2.060307973164624,
                        1.3610957975070113,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        56,
                        57,
                    },
                    (double[]){
                        1.7738880208445973,
                        -1.4622439449871554,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        55,
                        56,
                    },
                    (double[]){
                        0.40092507642645947,
                        1.30132084294919,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        55,
                    },
                    (double[]){
                        0.7725302750281119,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        69,
                    },
                    (double[]){
                        2.4709345081678493,
                        -3.611456308493466,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        68,
                    },
                    (double[]){
                        2.116761295556742,
                        -3.3994082446135216,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        67,
                    },
                    (double[]){
                        2.401340208949079,
                        1.5004218601330948,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        66,
                    },
                    (double[]){
                        2.806699432378035,
                        -4.320617208397168,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        65,
                    },
                    (double[]){
                        2.4330175494819337,
                        -0.6644117144965903,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        64,
                    },
                    (double[]){
                        2.9644354208163897,
                        1.2936572409719918,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        63,
                    },
                    (double[]){
                        2.8534074749247753,
                        -2.6630528229120003,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        62,
                    },
                    (double[]){
                        2.917005819940553,
                        -3.130228096731588,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        61,
                    },
                    (double[]){
                        2.4689218971995435,
                        -3.6622483339308376,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        60,
                    },
                    (double[]){
                        2.6830275017396725,
                        0.8715528912111359,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        58,
                        59,
                    },
                    (double[]){
                        1.9123867104221945,
                        0.6700184015846862,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        57,
                        58,
                    },
                    (double[]){
                        1.8630937246727894,
                        -2.058230323417873,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        70,
                    },
                    (double[]){
                        2.607663680630556,
                        -0.7260529055906411,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        69,
                    },
                    (double[]){
                        2.5788225385341947,
                        -2.497074697731434,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        68,
                    },
                    (double[]){
                        2.8549045085383784,
                        -3.0366805351535624,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        67,
                    },
                    (double[]){
                        2.560898840319293,
                        -4.049453313489588,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        66,
                    },
                    (double[]){
                        2.9558299086697324,
                        -2.0019136479802815,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        65,
                    },
                    (double[]){
                        2.443039209739204,
                        -2.2670625273925613,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        64,
                    },
                    (double[]){
                        2.4008571443666784,
                        -2.666608462984672,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        63,
                    },
                    (double[]){
                        2.0268090942852437,
                        -1.6653407982340809,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        62,
                    },
                    (double[]){
                        2.5126875155353847,
                        -0.3704071583694917,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        60,
                        61,
                    },
                    (double[]){
                        2.092112316122296,
                        -4.3974729623961375,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        59,
                        60,
                    },
                    (double[]){
                        2.4775932154490317,
                        -3.4852171295455987,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        71,
                    },
                    (double[]){
                        2.8820489148983413,
                        -0.6634677891467184,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        70,
                    },
                    (double[]){
                        2.1431047177354285,
                        -1.742716282086005,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        69,
                    },
                    (double[]){
                        2.4776852580709052,
                        0.013088672893685871,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        68,
                    },
                    (double[]){
                        2.6358083005341832,
                        0.43081097265634805,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        67,
                    },
                    (double[]){
                        2.891037048353161,
                        -0.5524367127945862,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        66,
                    },
                    (double[]){
                        2.3927810471774418,
                        -2.7901063729139386,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        65,
                    },
                    (double[]){
                        2.3470710387315528,
                        -2.5485335795427853,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        64,
                    },
                    (double[]){
                        2.1541026579290588,
                        0.005703902385155057,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        62,
                        63,
                    },
                    (double[]){
                        2.889316452874932,
                        -2.511419831239073,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        61,
                        62,
                    },
                    (double[]){
                        1.0556964301253118,
                        -2.028590933952694,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        71,
                    },
                    (double[]){
                        2.663235060924524,
                        -3.8507522520765938,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        70,
                    },
                    (double[]){
                        2.663836581522742,
                        -2.749890320105498,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        69,
                    },
                    (double[]){
                        2.3804578410822623,
                        -1.11496260719794,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        68,
                    },
                    (double[]){
                        2.784959063839767,
                        0.8615622667650142,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        67,
                    },
                    (double[]){
                        2.1274866778465364,
                        -0.3787346374505922,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        66,
                    },
                    (double[]){
                        2.673361100896759,
                        1.0203828880815822,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        64,
                        65,
                    },
                    (double[]){
                        2.3756601361582312,
                        -0.1499076268739099,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        63,
                        64,
                    },
                    (double[]){
                        0.571242861298048,
                        -4.03318556075366,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        63,
                    },
                    (double[]){
                        -1.335928075890712,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        71,
                    },
                    (double[]){
                        1.0530220460826385,
                        0.3696372449342398,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        70,
                    },
                    (double[]){
                        1.3032766590386933,
                        -4.439798759290859,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        69,
                    },
                    (double[]){
                        2.470996102016718,
                        -0.42832096870321035,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        68,
                    },
                    (double[]){
                        2.1670853367116996,
                        -2.8334269261768608,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        66,
                        67,
                    },
                    (double[]){
                        1.2542848474552848,
                        0.10489161295510652,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        65,
                        66,
                    },
                    (double[]){
                        1.4115014914283335,
                        -2.4729068222341573,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        71,
                    },
                    (double[]){
                        1.606201202881536,
                        0.7670021720836164,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        70,
                    },
                    (double[]){
                        2.073595440854907,
                        1.0401069944013188,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        68,
                        69,
                    },
                    (double[]){
                        1.7814090763756307,
                        0.1486576507052504,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        67,
                        68,
                    },
                    (double[]){
                        2.047517230414691,
                        -2.4275118424122653,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        68,
                    },
                    (double[]){
                        2.9534883137158237,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        70,
                        71,
                    },
                    (double[]){
                        1.5736882516188928,
                        -0.0283303185445416,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        69,
                        70,
                    },
                    (double[]){
                        0.8378175060606861,
                        -2.199610613967046,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        69,
                    },
                    (double[]){
                        -1.6809123037964833,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        2.5326753771986494,
                        -0.4223071786852588,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        2.7936798380851595,
                        -4.23392723716433,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        2.6519811060877165,
                        -0.9658115691657598,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        3.093006393626582,
                        -3.191301868458355,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        2.9942941609022853,
                        -0.24694274971016683,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        2.6135793105238347,
                        -1.9075214022772928,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        3.0372706808401624,
                        1.2253641838201594,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        15,
                    },
                    (double[]){
                        2.5639434559624834,
                        -1.1476109048057501,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        16,
                    },
                    (double[]){
                        2.8288982060198475,
                        1.501040820991693,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        17,
                    },
                    (double[]){
                        2.776083247497897,
                        1.3414436419646063,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        18,
                    },
                    (double[]){
                        2.8554729815800215,
                        -2.1297622278845703,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        19,
                    },
                    (double[]){
                        2.8854206084721232,
                        -2.8866215603154,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        20,
                    },
                    (double[]){
                        2.56055729123577,
                        -4.68394911075948,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        21,
                    },
                    (double[]){
                        2.613635714662012,
                        1.0578293928445563,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        23,
                    },
                    (double[]){
                        2.6996713148676816,
                        1.368719083807946,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        24,
                    },
                    (double[]){
                        2.656413457296041,
                        1.4414963822356137,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        25,
                    },
                    (double[]){
                        2.790159216334415,
                        -1.7623776587436342,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        26,
                    },
                    (double[]){
                        2.482044775685297,
                        -4.026910927117437,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        27,
                    },
                    (double[]){
                        2.8366999476608026,
                        -2.3245631113340015,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        28,
                    },
                    (double[]){
                        2.9810899738033987,
                        -2.5522542560150634,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        29,
                    },
                    (double[]){
                        2.9072225034862678,
                        0.9903412450993394,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        30,
                    },
                    (double[]){
                        2.1915138520027018,
                        0.3667812575679914,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        31,
                    },
                    (double[]){
                        2.5110896423999205,
                        0.37162234942138794,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        32,
                    },
                    (double[]){
                        2.3289778355106927,
                        -3.550440306662404,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        33,
                    },
                    (double[]){
                        2.4709345081678493,
                        -3.611456308493466,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        34,
                    },
                    (double[]){
                        2.607663680630556,
                        -0.7260529055906411,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        35,
                    },
                    (double[]){
                        2.8820489148983413,
                        -0.6634677891467184,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        2.879342716624786,
                        -0.4525440721328764,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        2.4893168602560802,
                        -0.22800979792397813,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        2.803681647999335,
                        -2.958594288527055,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        2.4091658522597847,
                        -3.4578994654340747,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        2.7247395605607543,
                        -4.4872328211397186,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        2.8790070632674674,
                        -4.221946272798072,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        1,
                    },
                    (double[]){
                        2.678508671594563,
                        -2.348064168469377,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        0,
                    },
                    (double[]){
                        2.4619775459668274,
                        0.19165231780468384,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        2.3922589494810262,
                        -1.3652511343822331,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        2.8753609815971477,
                        -1.7984089279418476,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        2.777936028078684,
                        -0.44455726716064414,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        3.033612753011489,
                        -1.6150719206515722,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        2.9449184396753116,
                        -2.745064125788979,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        2.790489413242308,
                        -1.4998597053586074,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        2.915695893804007,
                        -2.0096507734874045,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        15,
                    },
                    (double[]){
                        2.2324382297414878,
                        -4.072104540538202,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        16,
                    },
                    (double[]){
                        3.001654906200765,
                        -2.975087618010079,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        17,
                    },
                    (double[]){
                        2.678329118768442,
                        -2.7132267950870546,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        18,
                    },
                    (double[]){
                        2.5858053398300305,
                        -2.0771787182425085,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        19,
                    },
                    (double[]){
                        2.307964013057533,
                        0.5161854834642918,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        20,
                    },
                    (double[]){
                        2.8949745624652063,
                        0.010906733766737897,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        22,
                    },
                    (double[]){
                        2.7096494726210216,
                        0.6695373231143944,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        23,
                    },
                    (double[]){
                        2.9369850060196536,
                        -1.2740931299119762,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        24,
                    },
                    (double[]){
                        2.8651056318325803,
                        -0.4937387658851402,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        25,
                    },
                    (double[]){
                        3.0164526539292336,
                        0.19344682089943088,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        26,
                    },
                    (double[]){
                        3.0113716935235577,
                        -3.6783014277973844,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        27,
                    },
                    (double[]){
                        2.3563237812351794,
                        -2.583655719545386,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        28,
                    },
                    (double[]){
                        2.6698657880840844,
                        -0.5307490335321754,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        29,
                    },
                    (double[]){
                        2.4025083574976245,
                        0.8856199847285269,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        30,
                    },
                    (double[]){
                        2.5236797873191374,
                        -1.860523407515669,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        31,
                    },
                    (double[]){
                        2.988633215991043,
                        -3.390254936871692,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        32,
                    },
                    (double[]){
                        2.116761295556742,
                        -3.3994082446135216,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        33,
                    },
                    (double[]){
                        2.5788225385341947,
                        -2.497074697731434,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        34,
                    },
                    (double[]){
                        2.1431047177354285,
                        -1.742716282086005,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        35,
                    },
                    (double[]){
                        2.663235060924524,
                        -3.8507522520765938,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        2.3991896375827046,
                        -4.683504789110083,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        2.199344005041305,
                        -1.6791448997377585,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        2.57117402467766,
                        -3.504601857941653,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        2.847287476443901,
                        0.6011701969170322,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        2.603429551783166,
                        -0.48290440957584213,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        2.63279092839864,
                        -2.3865285553256035,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        1,
                    },
                    (double[]){
                        2.879275061845869,
                        -0.17404427746497775,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        0,
                    },
                    (double[]){
                        2.120542109271803,
                        -1.4947542893601702,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        2.4202445206466274,
                        1.0017712500167306,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        3.0205789952649815,
                        -0.42263728425826574,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        2.757316449366157,
                        1.477259728653645,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        2.9000861530290565,
                        0.6934292985045118,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        2.7311642667418616,
                        -3.865302497986964,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        2.9541294333315413,
                        0.020648996036576772,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        2.511673024286581,
                        -3.9058847304825663,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        15,
                    },
                    (double[]){
                        2.6858304319380224,
                        0.37293767582821946,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        16,
                    },
                    (double[]){
                        2.748805365566814,
                        -3.8933020928839075,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        17,
                    },
                    (double[]){
                        2.7940270634369977,
                        1.5140615037443381,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        18,
                    },
                    (double[]){
                        3.0529263971988048,
                        -4.516384849733665,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        19,
                    },
                    (double[]){
                        2.967791085194453,
                        -0.3569945841166511,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        21,
                    },
                    (double[]){
                        2.7887057636942,
                        -2.7240395330940927,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        22,
                    },
                    (double[]){
                        2.9414852478555567,
                        -3.3777421763097326,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        23,
                    },
                    (double[]){
                        2.9265047714823758,
                        1.548577386983776,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        24,
                    },
                    (double[]){
                        2.931020232420629,
                        0.8812303766050569,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        25,
                    },
                    (double[]){
                        2.7999050613630097,
                        0.7178794463320433,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        26,
                    },
                    (double[]){
                        2.8420795312416636,
                        -1.6514976924642937,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        27,
                    },
                    (double[]){
                        2.758670357005995,
                        -0.9579283196751032,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        28,
                    },
                    (double[]){
                        2.720459240903196,
                        0.3814099489566223,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        29,
                    },
                    (double[]){
                        2.5708976998950135,
                        -0.4877927363512107,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        30,
                    },
                    (double[]){
                        2.6905258493118653,
                        0.8702980010646559,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        31,
                    },
                    (double[]){
                        2.401340208949079,
                        1.5004218601330948,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        32,
                    },
                    (double[]){
                        2.8549045085383784,
                        -3.0366805351535624,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        33,
                    },
                    (double[]){
                        2.4776852580709052,
                        0.013088672893685871,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        34,
                    },
                    (double[]){
                        2.663836581522742,
                        -2.749890320105498,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        35,
                    },
                    (double[]){
                        1.0530220460826385,
                        0.3696372449342398,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        2.944059947693303,
                        0.7283304960167793,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        2.7551883002864996,
                        -2.5882750306735023,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        3.0094104970386892,
                        1.4907374587246323,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        3.086549426825471,
                        0.4220545007719154,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        2.9444815992992375,
                        1.2359852388580324,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        2.5104193716296836,
                        -4.303051616875107,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        1,
                    },
                    (double[]){
                        2.3968847157699944,
                        -3.3386328118460495,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        0,
                    },
                    (double[]){
                        1.95093187506634,
                        -1.9147602123400869,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        2.804186993591348,
                        1.1975074021372731,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        2.6397691336811215,
                        -3.880386032069793,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        2.886364069210408,
                        1.5161538060701063,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        2.9217276294846224,
                        -2.470286452312313,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        2.863569587551981,
                        1.2305538277218941,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        2.698172240482159,
                        -0.28720351034326574,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        3.0044721432412955,
                        -3.7581945681911413,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        15,
                    },
                    (double[]){
                        3.054338009536866,
                        0.5056686186254127,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        16,
                    },
                    (double[]){
                        2.9464997073876997,
                        -0.02411759135282865,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        17,
                    },
                    (double[]){
                        2.542948625588145,
                        0.5016269515184759,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        18,
                    },
                    (double[]){
                        3.042663996981131,
                        -1.2284373726074378,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        20,
                    },
                    (double[]){
                        2.9099076374697077,
                        0.7358535687578036,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        21,
                    },
                    (double[]){
                        2.719739894778777,
                        -0.31213556032272405,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        22,
                    },
                    (double[]){
                        2.8577697455409847,
                        -1.742296565948419,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        23,
                    },
                    (double[]){
                        2.589780817468795,
                        -3.885679153180544,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        24,
                    },
                    (double[]){
                        2.5899808338179473,
                        -0.22588256812947982,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        25,
                    },
                    (double[]){
                        2.8160557474184134,
                        -0.8028452990394259,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        26,
                    },
                    (double[]){
                        2.6877735012378654,
                        -2.9093344351239763,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        27,
                    },
                    (double[]){
                        2.485531331098659,
                        -3.92893770554438,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        28,
                    },
                    (double[]){
                        2.816373606410593,
                        1.0688742893989622,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        29,
                    },
                    (double[]){
                        2.6844318584406115,
                        -4.564675106423861,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        30,
                    },
                    (double[]){
                        2.806699432378035,
                        -4.320617208397168,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        31,
                    },
                    (double[]){
                        2.560898840319293,
                        -4.049453313489588,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        32,
                    },
                    (double[]){
                        2.6358083005341832,
                        0.43081097265634805,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        33,
                    },
                    (double[]){
                        2.3804578410822623,
                        -1.11496260719794,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        34,
                    },
                    (double[]){
                        1.3032766590386933,
                        -4.439798759290859,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        35,
                    },
                    (double[]){
                        1.606201202881536,
                        0.7670021720836164,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        2.7190531376207785,
                        1.0460291029219175,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        2.7179106092677605,
                        -4.087186457313529,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        3.0372038930636243,
                        -2.5934167083669477,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        2.673758253812591,
                        0.10130316311107568,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        2.84329345981634,
                        -2.843029777702397,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        2.493270802674935,
                        1.1248411735464638,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        1,
                    },
                    (double[]){
                        2.3167212237591595,
                        -1.6147047410553894,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        0,
                    },
                    (double[]){
                        0.9583114747465359,
                        -4.310385413718229,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        2.370199846521615,
                        -0.21421050705225153,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        2.7189735902410175,
                        -3.322922687859119,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        3.0047004495328897,
                        -0.5829819452340257,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        2.743258840206361,
                        -1.0581922621459228,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        2.7991499199330794,
                        -3.3156598121276977,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        2.4095717733740605,
                        -3.9535004619322986,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        2.7163620781150195,
                        -3.077326479357918,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        15,
                    },
                    (double[]){
                        2.8593638706218587,
                        -4.064281953518923,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        16,
                    },
                    (double[]){
                        2.774096066627165,
                        -2.553314345413077,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        17,
                    },
                    (double[]){
                        2.9167979313849544,
                        -4.677841901332673,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        19,
                    },
                    (double[]){
                        2.8958715655881555,
                        -3.825238353187724,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        20,
                    },
                    (double[]){
                        2.5979849642649206,
                        -2.707063442866503,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        21,
                    },
                    (double[]){
                        2.428212976428276,
                        -4.607018589701775,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        22,
                    },
                    (double[]){
                        2.995644869786327,
                        -3.336380008998653,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        23,
                    },
                    (double[]){
                        2.8308216524283827,
                        -3.416960856178253,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        24,
                    },
                    (double[]){
                        2.9720035194730485,
                        0.6325693310707039,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        25,
                    },
                    (double[]){
                        2.71030799604598,
                        -3.587728453138113,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        26,
                    },
                    (double[]){
                        2.5295707377000927,
                        0.5193363188771558,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        27,
                    },
                    (double[]){
                        2.4293963019809577,
                        -1.3891203039279008,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        28,
                    },
                    (double[]){
                        2.776914025353056,
                        -1.5302459471957932,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        29,
                    },
                    (double[]){
                        2.4330175494819337,
                        -0.6644117144965903,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        30,
                    },
                    (double[]){
                        2.9558299086697324,
                        -2.0019136479802815,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        31,
                    },
                    (double[]){
                        2.891037048353161,
                        -0.5524367127945862,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        32,
                    },
                    (double[]){
                        2.784959063839767,
                        0.8615622667650142,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        33,
                    },
                    (double[]){
                        2.470996102016718,
                        -0.42832096870321035,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        34,
                    },
                    (double[]){
                        2.073595440854907,
                        1.0401069944013188,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        34,
                        35,
                    },
                    (double[]){
                        1.5736882516188928,
                        -0.0283303185445416,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        35,
                    },
                    (double[]){
                        -2.6255340949328962,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        2.6669272926480025,
                        0.624934571152719,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        2.9339700837996943,
                        0.17700520201339542,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        2.846449856661585,
                        1.0815920567010489,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        2.8046197137302755,
                        -1.3490917452134548,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        2.6146411037819903,
                        -3.9818016740519124,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        2.610687839494691,
                        -3.597879231066967,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        1,
                    },
                    (double[]){
                        2.655443053079464,
                        -2.634356498670199,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        0,
                    },
                    (double[]){
                        1.0663965248189058,
                        -1.890975964370857,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        2.993142198152061,
                        1.027383823218516,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        2.895363839617158,
                        -2.245386614211008,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        2.8008278626687657,
                        -4.612904224724799,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        2.474632586801765,
                        -4.457555665067929,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        2.816312690085415,
                        1.046853238568267,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        2.920714563965329,
                        -1.2617499631587137,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        3.0361683068334364,
                        -0.23815918646251522,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        15,
                    },
                    (double[]){
                        2.9810347121926317,
                        1.3117014425376952,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        16,
                    },
                    (double[]){
                        2.517292658846209,
                        0.648188954683421,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        18,
                    },
                    (double[]){
                        2.8313422693182453,
                        -2.682499764846015,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        19,
                    },
                    (double[]){
                        2.4073651680443042,
                        0.5633458501447084,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        20,
                    },
                    (double[]){
                        2.6141117307553565,
                        -0.9244345742049852,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        21,
                    },
                    (double[]){
                        3.043378864938286,
                        -1.8904581676720453,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        22,
                    },
                    (double[]){
                        2.720589946945526,
                        -4.134401505337911,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        23,
                    },
                    (double[]){
                        2.579987427558504,
                        -0.8508338287522145,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        24,
                    },
                    (double[]){
                        2.7520613861889345,
                        -0.21810887780280486,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        25,
                    },
                    (double[]){
                        2.796347014267797,
                        -3.5314172117982996,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        26,
                    },
                    (double[]){
                        2.671194891555819,
                        -0.4307430478604888,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        27,
                    },
                    (double[]){
                        2.6689566868670447,
                        0.3473282981908383,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        28,
                    },
                    (double[]){
                        2.9644354208163897,
                        1.2936572409719918,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        29,
                    },
                    (double[]){
                        2.443039209739204,
                        -2.2670625273925613,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        30,
                    },
                    (double[]){
                        2.3927810471774418,
                        -2.7901063729139386,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        31,
                    },
                    (double[]){
                        2.1274866778465364,
                        -0.3787346374505922,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        32,
                    },
                    (double[]){
                        2.1670853367116996,
                        -2.8334269261768608,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        32,
                        33,
                    },
                    (double[]){
                        1.7814090763756307,
                        0.1486576507052504,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        33,
                        34,
                    },
                    (double[]){
                        0.8378175060606861,
                        -2.199610613967046,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        34,
                    },
                    (double[]){
                        -1.1306334777407427,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        2.649560570512981,
                        -0.20231074349886113,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        2.6983275713553487,
                        -0.08326505010805052,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        3.009212840046099,
                        -1.9284748249314345,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        2.930370487046188,
                        -1.1775214901431144,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        2.2060630667073586,
                        -1.9603575639466397,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        2.4347375029293303,
                        -0.5875334950068896,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        1,
                    },
                    (double[]){
                        2.3913134159015677,
                        0.32230026788627586,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        0,
                    },
                    (double[]){
                        2.4258226584048357,
                        1.5444207955999585,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        2.58729503327512,
                        0.36958432833200683,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        2.7450385171435814,
                        -1.224847745416179,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        2.9442014677102697,
                        -3.8669061579100363,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        2.317675936208363,
                        0.05078098373074735,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        2.931109968877068,
                        -2.7075440090493554,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        2.7193724290264054,
                        -4.053425039917732,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        2.4284977478770533,
                        -4.677668636681988,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        15,
                    },
                    (double[]){
                        3.0875221852263897,
                        0.868734712094847,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        17,
                    },
                    (double[]){
                        2.749467203365316,
                        -0.6991642603202927,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        18,
                    },
                    (double[]){
                        2.6663390772332187,
                        -1.5133740543649359,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        19,
                    },
                    (double[]){
                        2.7083216942087933,
                        -1.008546740183603,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        20,
                    },
                    (double[]){
                        2.7549108182199697,
                        -1.6101309210564447,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        21,
                    },
                    (double[]){
                        2.708431532260987,
                        0.1744292287149929,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        22,
                    },
                    (double[]){
                        2.5590078715506346,
                        0.6478487518054292,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        23,
                    },
                    (double[]){
                        2.9262051344158437,
                        -0.7755886161512453,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        24,
                    },
                    (double[]){
                        2.6769851222602288,
                        -4.212865360649664,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        25,
                    },
                    (double[]){
                        2.4497774635599256,
                        -4.3513308042320835,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        26,
                    },
                    (double[]){
                        2.5003097674693895,
                        -4.457119708805836,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        27,
                    },
                    (double[]){
                        2.8534074749247753,
                        -2.6630528229120003,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        28,
                    },
                    (double[]){
                        2.4008571443666784,
                        -2.666608462984672,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        29,
                    },
                    (double[]){
                        2.3470710387315528,
                        -2.5485335795427853,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        30,
                    },
                    (double[]){
                        2.673361100896759,
                        1.0203828880815822,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        30,
                        31,
                    },
                    (double[]){
                        1.2542848474552848,
                        0.10489161295510652,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        31,
                        32,
                    },
                    (double[]){
                        2.047517230414691,
                        -2.4275118424122653,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        32,
                    },
                    (double[]){
                        2.9534883137158237,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        2.8202223681062213,
                        -2.032713052950255,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        2.8065254651438583,
                        1.527407044185673,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        2.545027037440552,
                        -2.1144741209586835,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        2.6225831314470476,
                        -3.8472971764887363,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        2.6088107706431525,
                        -1.5818996221668449,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        2.307554627233103,
                        -3.0176692912733563,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        1,
                    },
                    (double[]){
                        2.0305341744077303,
                        -1.4105888421532455,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        0,
                    },
                    (double[]){
                        2.0227471212280315,
                        1.3684084591698684,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        2.823919288843189,
                        -4.383023815954063,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        2.613571864571477,
                        -4.517868017833777,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        2.869906185659958,
                        -3.5548498695814805,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        2.9301659039597925,
                        -4.683812694573707,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        2.743630036564101,
                        -2.817849408379219,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        2.7589322123480766,
                        -1.1848321448026355,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        14,
                    },
                    (double[]){
                        2.7677216482597005,
                        -0.857885283723141,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        16,
                    },
                    (double[]){
                        2.8656263217101317,
                        -2.585230027503467,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        17,
                    },
                    (double[]){
                        2.8386818136081517,
                        -3.6923848663875374,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        18,
                    },
                    (double[]){
                        2.7663143390426415,
                        -3.732807910158822,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        19,
                    },
                    (double[]){
                        2.8216712958744217,
                        -2.337468754379637,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        20,
                    },
                    (double[]){
                        2.784005714402471,
                        0.1295558595107833,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        21,
                    },
                    (double[]){
                        2.911592729628225,
                        0.6966293655419813,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        22,
                    },
                    (double[]){
                        2.775329628642072,
                        -0.15109849431860534,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        23,
                    },
                    (double[]){
                        2.7032476978451534,
                        -2.33827893309296,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        24,
                    },
                    (double[]){
                        2.9457108778383483,
                        -2.8260185403413436,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        25,
                    },
                    (double[]){
                        2.871866160447497,
                        -0.7345078552580117,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        26,
                    },
                    (double[]){
                        2.917005819940553,
                        -3.130228096731588,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        27,
                    },
                    (double[]){
                        2.0268090942852437,
                        -1.6653407982340809,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        28,
                    },
                    (double[]){
                        2.1541026579290588,
                        0.005703902385155057,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        28,
                        29,
                    },
                    (double[]){
                        2.3756601361582312,
                        -0.1499076268739099,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        29,
                        30,
                    },
                    (double[]){
                        1.4115014914283335,
                        -2.4729068222341573,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        3.017578119525451,
                        -1.7061214882078568,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        2.712967334243969,
                        -4.511822301400015,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        2.4566768973066684,
                        -3.7542518567625516,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        2.424542253526856,
                        -2.4128990447024385,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        2.9121225698037927,
                        -1.764033741256254,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        2.5725177335907885,
                        -0.4434307989825108,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        1,
                    },
                    (double[]){
                        1.9093734253148449,
                        -1.0177845746209546,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        0,
                    },
                    (double[]){
                        2.172445878632686,
                        -0.927513722982384,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        2.907625344232878,
                        0.1795712708003292,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        2.9365212943109804,
                        -3.259640175969666,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        2.824774588550247,
                        -1.46046255057644,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        3.030764944802419,
                        -0.46912609670142125,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        2.485728613395424,
                        -4.16338891214464,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        13,
                    },
                    (double[]){
                        3.062161417988011,
                        -4.151710096799841,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        15,
                    },
                    (double[]){
                        2.6345426644926073,
                        -2.5625199403259784,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        16,
                    },
                    (double[]){
                        2.8817666489302756,
                        1.4941746366693445,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        17,
                    },
                    (double[]){
                        2.8419167146962794,
                        -1.7044243230395955,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        18,
                    },
                    (double[]){
                        2.846004282959235,
                        -0.6557930292154726,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        19,
                    },
                    (double[]){
                        2.5677852886433863,
                        -3.5620616274777173,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        20,
                    },
                    (double[]){
                        2.727521116259641,
                        0.40520856976312114,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        21,
                    },
                    (double[]){
                        2.6094720237432494,
                        -3.981822332872958,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        22,
                    },
                    (double[]){
                        2.5476073014626874,
                        1.0082697712244624,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        23,
                    },
                    (double[]){
                        2.9590613870838105,
                        -2.504509988456335,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        24,
                    },
                    (double[]){
                        2.773333027155682,
                        0.35179789177437004,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        25,
                    },
                    (double[]){
                        2.4689218971995435,
                        -3.6622483339308376,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        26,
                    },
                    (double[]){
                        2.5126875155353847,
                        -0.3704071583694917,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        26,
                        27,
                    },
                    (double[]){
                        2.889316452874932,
                        -2.511419831239073,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        27,
                        28,
                    },
                    (double[]){
                        0.571242861298048,
                        -4.03318556075366,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        3.0466492537345533,
                        1.2915453210130425,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        2.5504109763848843,
                        -3.7776089068259586,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        2.6484441557498957,
                        -1.8682289912243395,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        2.0149953832403265,
                        -0.5237207454390396,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        2.2945026487329057,
                        -4.596709915381738,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        2.419560190530837,
                        1.2325490983646956,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        1,
                    },
                    (double[]){
                        2.4295597977512244,
                        -0.8152910106979927,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        0,
                    },
                    (double[]){
                        1.8350276077342873,
                        -2.9199196287314013,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        2.519210737722506,
                        -3.004716297204361,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        2.8900807950164786,
                        -1.9085493235914246,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        2.5170275273479783,
                        1.312869484697961,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        2.414683370699701,
                        -3.942444054292303,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        12,
                    },
                    (double[]){
                        2.7354536701198144,
                        -3.4971923231155655,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        14,
                    },
                    (double[]){
                        2.851079448807529,
                        1.2654363027034496,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        15,
                    },
                    (double[]){
                        2.9125745000996197,
                        -2.5342375911219643,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        16,
                    },
                    (double[]){
                        2.7037375288857315,
                        -0.5021187856334819,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        17,
                    },
                    (double[]){
                        2.6411251920815197,
                        -0.28839221593751785,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        18,
                    },
                    (double[]){
                        2.968137940933438,
                        -4.601482305488428,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        19,
                    },
                    (double[]){
                        1.967410702001562,
                        -1.6080762518316918,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        20,
                    },
                    (double[]){
                        2.8648036946046274,
                        0.08045252085793386,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        21,
                    },
                    (double[]){
                        2.543465779896254,
                        0.5295731844759368,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        22,
                    },
                    (double[]){
                        2.8538857982139048,
                        -1.037800832610594,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        23,
                    },
                    (double[]){
                        2.4096198462555485,
                        -3.8630961391100334,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        24,
                    },
                    (double[]){
                        2.6830275017396725,
                        0.8715528912111359,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        24,
                        25,
                    },
                    (double[]){
                        2.092112316122296,
                        -4.3974729623961375,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        25,
                        26,
                    },
                    (double[]){
                        1.0556964301253118,
                        -2.028590933952694,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        25,
                    },
                    (double[]){
                        0.7763280045616187,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        26,
                    },
                    (double[]){
                        0.7325173381987065,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        2.739784569227518,
                        -0.07659879917098755,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        2.6443629603436487,
                        -0.4569305441184859,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        3.0224054656754467,
                        -0.5941459575267745,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        2.5842394765754255,
                        -2.7438035170105337,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        2.9038047795522215,
                        1.1495651486333536,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        1.8095336894868959,
                        -4.035625150226755,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        1,
                    },
                    (double[]){
                        2.808040235920636,
                        -1.065016823876206,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        0,
                    },
                    (double[]){
                        2.0457699832487504,
                        0.47143886669291435,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        2.8684479127776594,
                        -4.586095585027394,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        2.745868719138243,
                        -0.4235232425328843,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        2.588346867143977,
                        0.5419704825488645,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        11,
                    },
                    (double[]){
                        2.4594885652629905,
                        -2.068564039159127,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        13,
                    },
                    (double[]){
                        2.8576632893378977,
                        -4.704999732684566,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        14,
                    },
                    (double[]){
                        2.8262037107185516,
                        -3.3340824194591336,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        15,
                    },
                    (double[]){
                        2.6740553288034468,
                        0.7439216608233821,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        16,
                    },
                    (double[]){
                        2.6536667819373836,
                        -2.704824712278417,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        17,
                    },
                    (double[]){
                        2.2820058195353012,
                        -0.2774181368443447,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        18,
                    },
                    (double[]){
                        2.3652924001986406,
                        -2.4146900012797916,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        19,
                    },
                    (double[]){
                        2.7424682843972565,
                        0.8737800593538672,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        20,
                    },
                    (double[]){
                        2.1929262497787625,
                        -2.210765004224075,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        21,
                    },
                    (double[]){
                        2.1614964837533006,
                        -2.44514085918127,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        22,
                    },
                    (double[]){
                        2.060307973164624,
                        1.3610957975070113,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        22,
                        23,
                    },
                    (double[]){
                        1.9123867104221945,
                        0.6700184015846862,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        23,
                        24,
                    },
                    (double[]){
                        2.4775932154490317,
                        -3.4852171295455987,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        23,
                    },
                    (double[]){
                        2.5924952171931315,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        2.5938246867490076,
                        0.41673895204009637,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        3.0024239200004366,
                        -3.8863564484682773,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        2.7851758749226265,
                        1.5039647590748633,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        2.883230332260518,
                        1.3441897894439214,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        2.747392852499555,
                        -4.185439133911553,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        1.8698921314341648,
                        0.8990260073567118,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        1,
                    },
                    (double[]){
                        1.3824196931257122,
                        -0.4427937199631886,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        0,
                    },
                    (double[]){
                        1.85697837848801,
                        -3.823570358826113,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        1.9990613767462997,
                        -4.1226431747204995,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        2.5997039021156736,
                        -3.975304665429338,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        10,
                    },
                    (double[]){
                        2.58468400111932,
                        -3.5977420227743586,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        12,
                    },
                    (double[]){
                        2.680244274781544,
                        -0.9191953313400474,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        13,
                    },
                    (double[]){
                        2.9486291911372153,
                        -2.2197023314620514,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        14,
                    },
                    (double[]){
                        2.707415219650358,
                        0.3603338644824263,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        15,
                    },
                    (double[]){
                        3.0050337077516374,
                        -3.8403376353020797,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        16,
                    },
                    (double[]){
                        2.8618301481034925,
                        -3.731494687822249,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        17,
                    },
                    (double[]){
                        2.7005446224370018,
                        -1.8422282409808455,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        18,
                    },
                    (double[]){
                        2.320508172421448,
                        -0.06422034117130604,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        19,
                    },
                    (double[]){
                        2.8887084587563026,
                        -4.451788061793018,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        20,
                    },
                    (double[]){
                        2.3315317568616507,
                        -0.6572578709569132,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        20,
                        21,
                    },
                    (double[]){
                        1.7738880208445973,
                        -1.4622439449871554,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        21,
                        22,
                    },
                    (double[]){
                        1.8630937246727894,
                        -2.058230323417873,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        3.0073226349639897,
                        -3.6147051653623725,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        2.7993985141692668,
                        -3.140748857459534,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        2.8860492839572376,
                        1.3576033662075035,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        2.5341231986689774,
                        1.4383962300349662,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        3.0419256105812273,
                        -2.951214608107607,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        2.436316500931743,
                        -1.7962446160248091,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        1,
                    },
                    (double[]){
                        1.834055095870482,
                        -3.3237354482391126,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        0,
                    },
                    (double[]){
                        0.9152513179553405,
                        -2.459334355652267,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        2.620273588717665,
                        -3.7343461864773815,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        9,
                    },
                    (double[]){
                        2.4926165091054364,
                        -2.037060050085576,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        11,
                    },
                    (double[]){
                        2.7533516238636953,
                        -0.9491026308244892,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        12,
                    },
                    (double[]){
                        2.4798692315333963,
                        -1.2877357768685183,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        13,
                    },
                    (double[]){
                        2.6639103402819426,
                        0.9878483294958627,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        14,
                    },
                    (double[]){
                        3.017211334920625,
                        -1.4326017618066866,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        15,
                    },
                    (double[]){
                        2.5298102667879414,
                        -2.972312847080187,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        16,
                    },
                    (double[]){
                        2.9625368121020044,
                        -4.482259561729266,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        17,
                    },
                    (double[]){
                        2.979929021266038,
                        -4.0925521973167776,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        18,
                    },
                    (double[]){
                        2.20474139038771,
                        -4.003212313992163,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        18,
                        19,
                    },
                    (double[]){
                        2.760088116520873,
                        -0.9631507107937185,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        19,
                        20,
                    },
                    (double[]){
                        0.40092507642645947,
                        1.30132084294919,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        2.475404747487059,
                        -2.101628188777282,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        3.0428582147740433,
                        -4.631024430774277,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        2.781607979175291,
                        -3.5477676198112276,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        1.8477817821826783,
                        0.38966439544398424,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        3.1007456041872024,
                        -0.7322881466876723,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        1.8030745898778033,
                        -3.7652110506944445,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        1,
                    },
                    (double[]){
                        2.9238415834030613,
                        -0.9760751245139779,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        0,
                    },
                    (double[]){
                        1.1379270381117592,
                        -0.5261716958513794,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        8,
                    },
                    (double[]){
                        2.7813304267248626,
                        1.1745383479770752,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        7,
                    },
                    (double[]){
                        2.717916531749616,
                        -1.927132542919367,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        6,
                    },
                    (double[]){
                        2.703478864258205,
                        -1.0605409461818711,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        5,
                    },
                    (double[]){
                        2.618102623098778,
                        1.5246585168711735,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        4,
                    },
                    (double[]){
                        2.5978355600846195,
                        -1.752850679728389,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        3,
                    },
                    (double[]){
                        2.7072532555729305,
                        -0.2703049957542234,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        2,
                    },
                    (double[]){
                        2.640559198072321,
                        1.2426914634545239,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        1,
                    },
                    (double[]){
                        2.0930630358400233,
                        -4.533183474048242,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        0,
                    },
                    (double[]){
                        0.31682329698762796,
                        -2.4919415397021454,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        10,
                    },
                    (double[]){
                        2.7921474004478477,
                        -3.107713396346086,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        11,
                    },
                    (double[]){
                        2.830155913179915,
                        0.6292333358587832,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        12,
                    },
                    (double[]){
                        2.951618302301949,
                        1.1209587172812956,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        13,
                    },
                    (double[]){
                        2.477949351237913,
                        0.015328422591699864,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        14,
                    },
                    (double[]){
                        2.2464886534905975,
                        -0.13008711868035938,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        15,
                    },
                    (double[]){
                        2.790836153509008,
                        -2.3965127076766657,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        16,
                    },
                    (double[]){
                        2.433255702121207,
                        -4.131258303203252,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        16,
                        17,
                    },
                    (double[]){
                        2.898774292544521,
                        -0.9486275774850728,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        17,
                        18,
                    },
                    (double[]){
                        0.7709075291363177,
                        -4.107327589281249,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        18,
                    },
                    (double[]){
                        -1.2286416531823288,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        9,
                    },
                    (double[]){
                        2.5202832434520346,
                        -2.59501028597785,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        8,
                    },
                    (double[]){
                        2.7848500793290007,
                        0.03344164557895213,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        7,
                    },
                    (double[]){
                        2.691710610681685,
                        -2.381590646391839,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        6,
                    },
                    (double[]){
                        2.514409968774285,
                        -2.026534545349116,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        5,
                    },
                    (double[]){
                        2.9801106538067295,
                        -3.9628352870435024,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        4,
                    },
                    (double[]){
                        2.61778895458567,
                        -2.01933940769691,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        2,
                        3,
                    },
                    (double[]){
                        1.9381688030086426,
                        0.5643524965780573,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        1,
                        2,
                    },
                    (double[]){
                        2.4234077043916438,
                        1.5679908851799338,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        2,
                    },
                    (double[]){
                        -2.585846253465858,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        10,
                    },
                    (double[]){
                        2.4834145183925074,
                        -3.38207369739457,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        11,
                    },
                    (double[]){
                        2.856782488922934,
                        -2.4185835197004923,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        12,
                    },
                    (double[]){
                        2.699033703759462,
                        -0.28034190495011657,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        13,
                    },
                    (double[]){
                        2.5960226542831277,
                        -1.584165592052527,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        14,
                    },
                    (double[]){
                        2.978302131339481,
                        -2.0624652997656865,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        14,
                        15,
                    },
                    (double[]){
                        2.0915685637206947,
                        -1.3011102353420392,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        15,
                        16,
                    },
                    (double[]){
                        1.3140783372057763,
                        -1.1765341254204567,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        16,
                    },
                    (double[]){
                        -2.1463397672114373,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        9,
                    },
                    (double[]){
                        2.211137114526412,
                        -0.793815318042057,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        8,
                    },
                    (double[]){
                        2.453102195575799,
                        -3.903876956520319,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        7,
                    },
                    (double[]){
                        2.2172038902859943,
                        -0.9834826472683712,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        6,
                    },
                    (double[]){
                        2.5977771014683078,
                        -3.339652222642604,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        4,
                        5,
                    },
                    (double[]){
                        1.3447948547353714,
                        0.15478764923214405,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        3,
                        4,
                    },
                    (double[]){
                        2.5013939760235946,
                        -4.267135210767637,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        3,
                    },
                    (double[]){
                        -0.4537570254817901,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        4,
                    },
                    (double[]){
                        -2.7102468672101976,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        10,
                    },
                    (double[]){
                        2.0943581648264566,
                        -0.36872052088278795,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        11,
                    },
                    (double[]){
                        2.5780455790590038,
                        0.767086092322542,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        12,
                    },
                    (double[]){
                        2.7573366880722663,
                        -2.6784726300871524,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        12,
                        13,
                    },
                    (double[]){
                        2.6692297612817732,
                        0.49309177298598916,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        13,
                        14,
                    },
                    (double[]){
                        1.9406612399783745,
                        -4.5272000249726805,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        13,
                    },
                    (double[]){
                        1.2257299215679558,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        14,
                    },
                    (double[]){
                        -1.7266479299310638,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        9,
                    },
                    (double[]){
                        2.566546691458132,
                        -0.7222944905933069,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        8,
                    },
                    (double[]){
                        1.6053584531614993,
                        -0.14949961503787002,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        6,
                        7,
                    },
                    (double[]){
                        2.9786587773229534,
                        -2.8371063954572575,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        5,
                        6,
                    },
                    (double[]){
                        1.9497920514489435,
                        1.366786495526918,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        5,
                    },
                    (double[]){
                        -1.8915081016626214,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        10,
                    },
                    (double[]){
                        3.0051008161739956,
                        -4.667263252762956,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        10,
                        11,
                    },
                    (double[]){
                        2.2569670051611777,
                        -1.6737072212867874,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        11,
                        12,
                    },
                    (double[]){
                        1.45634970258828,
                        1.0945920219840315,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        8,
                        9,
                    },
                    (double[]){
                        2.918850501259475,
                        -1.8905119389698948,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        7,
                        8,
                    },
                    (double[]){
                        2.2910638814623088,
                        -0.14445737979806794,
                    });
    qk_circuit_gate(qc, QkGate_XXPlusYY,
                    (uint32_t[]){
                        9,
                        10,
                    },
                    (double[]){
                        0.8695858645683318,
                        -3.975521650689593,
                    });
    qk_circuit_gate(qc, QkGate_Phase,
                    (uint32_t[]){
                        9,
                    },
                    (double[]){
                        2.659526331069742,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        0,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        1.277373699667245,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        1,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -1.3100023053052796,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        10,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -2.8324030772475606,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        11,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -2.3519311962914298,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        12,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        1.4190917669777106,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        15,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        0.8915145183315305,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        17,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        2.51539187335296,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        19,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        0.772530275028112,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        20,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -0.28845554722144096,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        21,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -1.8616558788051256,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        22,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        2.8933960390914386,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        24,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        0.6823153644356212,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        27,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -1.335928075890712,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        28,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        0.9128503567138981,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        29,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        3.0888539108807755,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        30,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        0.026009572767077938,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        31,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        2.3942241320715176,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        33,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -1.6809123037964833,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        37,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -1.3100023053052796,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        38,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -2.585846253465858,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        40,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -2.7102468672101976,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        41,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -1.8915081016626214,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        42,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -0.29868949422049607,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        43,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        0.46832792550002456,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        44,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        0.17716848847259303,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        45,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        2.659526331069742,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        46,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -2.8324030772475606,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        48,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        1.4190917669777106,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        50,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -1.7266479299310638,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        54,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -1.2286416531823288,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        56,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -0.28845554722144096,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        57,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -1.8616558788051256,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        58,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        2.8933960390914386,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        59,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        2.5924952171931315,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        6,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -0.29868949422049607,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        60,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        0.6823153644356212,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        61,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        0.7763280045616185,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        62,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        0.7325173381987065,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        64,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        0.9128503567138981,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        65,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        3.0888539108807755,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        66,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        0.026009572767077938,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        67,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        2.3942241320715176,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        7,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        0.46832792550002456,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        70,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -1.1306334777407425,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        71,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        -2.6255340949328962,
                    });
    qk_circuit_gate(qc, QkGate_U,
                    (uint32_t[]){
                        8,
                    },
                    (double[]){
                        0.0,
                        0.0,
                        0.17716848847259303,
                    });
}
