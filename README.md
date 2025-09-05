# qiskit-ibm-runtime rust

This repo contains a prototype of a qiskit-ibm-runtime rust implementation.
The goal here is **not** to be a fully fledged rust client that mirrors the
Python component but instead to provide a Rust library that exposes a C API
for users of the Qiskit C API to submit QuantumCircuit built from C for
execution and retrieve primitive results. The goal here is to use Rust to build
a safe API client for job submission and results processing for IBM's primitives
but expose that to C.

## Getting Started

### UNIX-like

#### Requirements

Compilation requires the following tools:
* git
* rustc
* cargo
* gcc
* cbindgen
* make
* cmake

#### Build

To build all tests, you can run the following command in qiskit-ibm-runtime-rs root,
```
mkdir build && cd build
cmake ..
make
```

#### Samples

There are several interactive commandline sample programs you can run in `/samples`.
These are built automatically to the root of the build folder.

#### Test

To run all tests, you can run the following command from within the build tree,
```
ctest
```