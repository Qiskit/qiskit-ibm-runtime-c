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

// Check that the angles of parametric gates survive QPY generation.

#include <qiskit.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern void generate_qpy(QkCircuit *circuit, char *filename);

#define QPY_FILENAME "test_params.qpy"

// Encode `value` as the 8 bytes QPY stores for an INSTRUCTION_PARAM double, least significant
// byte first. Built from the bit pattern, so the host's own byte order does not matter.
static void encode_double(double value, unsigned char out[8]) {
    uint64_t bits;
    memcpy(&bits, &value, sizeof(bits));
    for (int i = 0; i < 8; i++) {
        out[i] = (unsigned char)((bits >> (8 * i)) & 0xFF);
    }
}

static int contains(const unsigned char *haystack, size_t haystack_len,
                    const unsigned char *needle, size_t needle_len) {
    if (haystack_len < needle_len) {
        return 0;
    }
    for (size_t i = 0; i + needle_len <= haystack_len; i++) {
        if (memcmp(haystack + i, needle, needle_len) == 0) {
            return 1;
        }
    }
    return 0;
}

// Report whether `angle` reached the QPY payload, returning the number of failures found.
static int check_angle(const unsigned char *payload, size_t payload_len, double angle) {
    unsigned char encoded[8];
    encode_double(angle, encoded);

    if (!contains(payload, payload_len, encoded, sizeof(encoded))) {
        printf("FAIL: angle %g is missing from the QPY payload\n", angle);
        return 1;
    }
    return 0;
}

int main(void) {
    const double rz_angle = 0.05;
    const double rx_angle = 0.3;

    QkCircuit *qc = qk_circuit_new(2, 2);
    uint32_t rz_qubits[1] = {0};
    double rz_params[1] = {rz_angle};
    qk_circuit_gate(qc, QkGate_RZ, rz_qubits, rz_params);
    uint32_t rx_qubits[1] = {1};
    double rx_params[1] = {rx_angle};
    qk_circuit_gate(qc, QkGate_RX, rx_qubits, rx_params);
    qk_circuit_measure(qc, 0, 0);
    qk_circuit_measure(qc, 1, 1);

    generate_qpy(qc, QPY_FILENAME);
    qk_circuit_free(qc);

    FILE *file = fopen(QPY_FILENAME, "rb");
    if (file == NULL) {
        printf("FAIL: could not open %s\n", QPY_FILENAME);
        return 1;
    }

    unsigned char payload[8192];
    size_t read = fread(payload, 1, sizeof(payload), file);
    fclose(file);
    if (read == 0 || read == sizeof(payload)) {
        printf("FAIL: read %zu bytes from %s\n", read, QPY_FILENAME);
        return 1;
    }

    int failures = check_angle(payload, read, rz_angle) + check_angle(payload, read, rx_angle);

    if (failures > 0) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("rz(%g) and rx(%g) both round-trip through QPY\n", rz_angle, rx_angle);
    return 0;
}
