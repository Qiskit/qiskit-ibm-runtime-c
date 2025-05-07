from qiskit import qpy

with open("test.qpy", "rb") as fd:
    qc = qpy.load(fd)[0]

print(qc.count_ops())
