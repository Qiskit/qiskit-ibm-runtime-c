QISKIT_DIR_NAME = qiskit_c_lib
QISKIT_DIR = $(realpath -s ./$(QISKIT_DIR_NAME))
QISKIT_IBM_RUNTIME_RS_DIR = $(realpath -s .)
QISKIT_URL = https://github.com/Qiskit/qiskit.git

build: check_deps test_job_create test_qpy

test: build
	./test_qpy
	./test_job_create
	python3 test_qpy_load.py

check_deps:
	rustc --version
	cargo --version
	gcc --version
	cbindgen --version

test_job_create: $(QISKIT_IBM_RUNTIME_RS_DIR)/target/debug $(QISKIT_DIR)/dist/c
	export LD_LIBRARY_PATH=$(QISKIT_DIR)/dist/c/lib:$LD_LIBRARY_PATH
	export LD_LIBRARY_PATH=$(QISKIT_IBM_RUNTIME_RS_DIR)/target/debug:$LD_LIBRARY_PATH
	gcc test_job_create.c -ggdb  -o $@ -lqiskit_ibm_runtime -L./target/debug -I $(QISKIT_DIR)/dist/c/include -L $(QISKIT_DIR)/dist/c/lib -lqiskit

test_qpy: $(QISKIT_IBM_RUNTIME_RS_DIR)/target/debug $(QISKIT_DIR)/dist/c
	export LD_LIBRARY_PATH=$(QISKIT_DIR)/dist/c/lib:$LD_LIBRARY_PATH
	export LD_LIBRARY_PATH=$(QISKIT_IBM_RUNTIME_RS_DIR)/target/debug:$LD_LIBRARY_PATH
	gcc test_qpy.c  -o $@ -lqiskit_ibm_runtime -L./target/debug -I $(QISKIT_DIR)/dist/c/include -L $(QISKIT_DIR)/dist/c/lib -lqiskit

$(QISKIT_DIR)/dist/c: $(QISKIT_DIR)
	cd qiskit_c_lib && make c

$(QISKIT_DIR):
	git clone --depth 1 $(QISKIT_URL) qiskit_c_lib

$(QISKIT_IBM_RUNTIME_RS_DIR)/target/debug:
	cargo build

clean:
	cargo clean
	rm -rf $(QISKIT_DIR_NAME) test.qpy test_qpy test_job_create test_pitt.json test_before_json.qpy

.PHONY: test clean