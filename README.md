# qiskit-ibm-runtime rust

This repo contains a prototype of a qiskit-ibm-runtime rust implementation.
The goal here is **not** to be a fully fledged rust client that mirrors the
Python component but instead to provide a Rust library that exposes a C API
for users of the Qiskit C API to submit QuantumCircuit built from C for
execution and retrieve primitive results. The goal here is to use Rust to build
a safe API client for job submission and results processing for IBM's primitives
but expose that to C.
