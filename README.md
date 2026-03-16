# qiskit-ibm-runtime C

This repo contains a prototype of a qiskit-ibm-runtime Rust implementation
with a C interface.
The goal here is **not** to be a fully fledged rust client that mirrors the
Python component but instead to provide a Rust library that exposes a C API
for users of the Qiskit C API to submit QuantumCircuit built from C for
execution and retrieve primitive results. The goal here is to use Rust to build
a safe API client for job submission and results processing for IBM's primitives
but expose that to C.

> [!WARNING]
> This library is an early prototype and is still under active development.
> There are currently no backwards compatibility guarantees as the library
> is developed. This library also only supports a single Qiskit version at a
> time, currently only 2.2.0 is supported.


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

#### Running Sampler Examples

Before running the sampler examples, you must first create a config json file with  IBM Quantum account credentials at your home root directory `$HOME/.qiskit/qiskit-ibm.json`

##### Configuration Parameters

* `name`: Accepted examples `default-ibm-quantum-platform`, `defautt-ibm-cloud`, or `default`
* `token`: Your IBM Quantum API token
* `instance`: Quantum service instance CRN
* `channel`: The channel of the IBM Quantum API
* `url`: The url of the IBM Quantum API

##### Example of qiskit-ibm.json file:

Create the file `$HOME/.qiskit/qiskit-ibm.json` with the following structure:

```json
{
  "default-ibm-quantum-platform": {
    "token": "YOUR_IBM_QUANTUM_TOKEN",
    "instance": "YOUR_INSTANCE_CRN",
    "channel": "ibm_quantum",
    "url": "https://auth.quantum-computing.ibm.com/api"
  }
}
```

More details how to set your account can be found [here](https://github.com/Qiskit/qiskit-ibm-runtime?tab=readme-ov-file#save-your-account-on-disk)

##### Running the Examples

After configuring your account, execute the sampler examples as `./test_ghz_sampler_run`


#### License

This software is licensed under the Apache 2.0 license.

While most source files include a license header, some generated source files do not
(including sources from crates `ibm-quantum-platform-api`, `ibmcloud-global-search-api`,
and `ibmcloud-iam-api`). All of these sources are still provided under Apache 2.0.

