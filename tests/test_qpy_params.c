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

static int contains(const unsigned char *haystack, size_t haystack_len,
                    const unsigned char *needle, size_t needle_len) {
    for (size_t i = 0; i + needle_len <= haystack_len; i++) {
        if (memcmp(haystack + i, needle, needle_len) == 0) {
            return 1;
        }
    }
    return 0;
}

// Report whether `angle` reached the QPY payload, returning 1 if it did not.
static int check_angle(const unsigned char *payload, size_t payload_len, double angle) {
    // QPY stores instruction parameters little-endian, so build the expected bytes
    // from the IEEE-754 bit pattern rather than memcpy'ing the double -- that
    // keeps this check correct on a big-endian host, too.
    uint64_t bits;
    memcpy(&bits, &angle, sizeof(bits));
    unsigned char encoded[8];
    for (int i = 0; i < 8; i++) {
        encoded[i] = (unsigned char)((bits >> (8 * i)) & 0xFF);
    }

    if (contains(payload, payload_len, encoded, sizeof(encoded))) {
        return 0;
    }
    printf("FAIL: angle %g is missing from the QPY payload\n", angle);
    return 1;
}

int main(void) {
    const double rz_angle = 0.05;
    const double rx_angle = 0.3;

    QkCircuit *qc = qk_circuit_new(2, 2);
    qk_circuit_gate(qc, QkGate_RZ, (uint32_t[]){0}, (double[]){rz_angle});
    qk_circuit_gate(qc, QkGate_RX, (uint32_t[]){1}, (double[]){rx_angle});
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
        return 1;
    }
    printf("rz(%g) and rx(%g) both round-trip through QPY\n", rz_angle, rx_angle);
    return 0;
}
