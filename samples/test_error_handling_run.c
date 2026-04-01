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

/**
 * Test program to verify error message handling.
 * 
 * This program intentionally triggers configuration errors to test
 * that error messages are properly displayed.
 * 
 * Test scenarios:
 * 1. No credentials (neither env vars nor config file)
 * 2. Missing QISKIT_IBM_INSTANCE when QISKIT_IBM_TOKEN is set
 * 3. Invalid config file
 */

extern const char* qkrt_get_last_error(void);

void print_error(const char* context, int code) {
    fprintf(stderr, "\n=== Error in %s ===\n", context);
    fprintf(stderr, "Error code: %d\n", code);
    
    const char* error_msg = qkrt_get_last_error();
    if (error_msg != NULL && error_msg[0] != '\0') {
        fprintf(stderr, "Error details:\n%s\n", error_msg);
    } else {
        fprintf(stderr, "No detailed error message available.\n");
    }
    fprintf(stderr, "=========================\n\n");
}

int main(int argc, char *argv[]) {
    printf("=== Qiskit IBM Runtime Error Handling Test ===\n\n");
    
    printf("Test 1: Attempting to create service...\n");
    Service *service = NULL;
    int res = qkrt_service_new(&service);
    
    if (res != 0) {
        print_error("Service Creation", res);
        printf("This is expected if:\n");
        printf("  - No QISKIT_IBM_TOKEN environment variable is set\n");
        printf("  - No ~/.qiskit/qiskit-ibm.json config file exists\n");
        printf("  - QISKIT_IBM_TOKEN is set but QISKIT_IBM_INSTANCE is missing\n\n");
        
        printf("To fix this error, do one of the following:\n\n");
        printf("Option 1 - Use environment variables:\n");
        printf("  export QISKIT_IBM_TOKEN=\"your_token_here\"\n");
        printf("  export QISKIT_IBM_INSTANCE=\"your_instance_here\"\n");
        printf("  ./test_error_handling\n\n");
        
        printf("Option 2 - Create config file:\n");
        printf("  mkdir -p ~/.qiskit\n");
        printf("  cat > ~/.qiskit/qiskit-ibm.json << 'EOF'\n");
        printf("  {\n");
        printf("    \"default\": {\n");
        printf("      \"token\": \"your_token_here\",\n");
        printf("      \"instance\": \"your_instance_here\",\n");
        printf("      \"channel\": \"ibm_quantum\",\n");
        printf("      \"url\": \"https://auth.quantum-computing.ibm.com/api\"\n");
        printf("    }\n");
        printf("  }\n");
        printf("  EOF\n");
        printf("  ./test_error_handling_run\n\n");
        
        return res;
    }
    
    printf("✓ Service created successfully!\n");
    printf("This means your credentials are properly configured.\n\n");
    
    printf("Test 2: Attempting to search backends...\n");
    BackendSearchResults *results = NULL;
    res = qkrt_backend_search(&results, service);
    
    if (res != 0) {
        print_error("Backend Search", res);
        qkrt_service_free(service);
        return res;
    }
    
    uint64_t result_count = qkrt_backend_search_results_length(results);
    printf("✓ Found %llu backend(s)\n\n", result_count);
    
    if (result_count > 0) {
        Backend **backends = qkrt_backend_search_results_data(results);
        printf("Available backends:\n");
        for (uint64_t i = 0; i < result_count; i++) {
            printf("  [%llu] %s\n", i, qkrt_backend_name(backends[i]));
        }
    }
    
    qkrt_backend_search_results_free(results);
    qkrt_service_free(service);
    
    printf("\n=== All tests passed! ===\n");
    printf("Your configuration is working correctly.\n");
    
    return 0;
}
