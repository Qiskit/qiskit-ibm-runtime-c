from qiskit import qpy

with open("test.qpy", "rb") as fd:
    qc = qpy.load(fd)[0]

#print(qc)
print(qc.data[0].params)
print(qc.data[1].params)
print(qc.data[2].params)
print(qc.count_ops())
