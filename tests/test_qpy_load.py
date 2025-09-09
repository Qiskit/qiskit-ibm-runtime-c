# This code is part of Qiskit.
#
# (C) Copyright IBM 2025
#
# This code is licensed under the Apache License, Version 2.0. You may
# obtain a copy of this license in the LICENSE.txt file in the root directory
# of this source tree or at http://www.apache.org/licenses/LICENSE-2.0.
#
# Any modifications or derivative works of this code must retain this
# copyright notice, and modified files need to carry a notice indicating
# that they have been altered from the originals.

from qiskit import qpy

with open("test.qpy", "rb") as fd:
    qc = qpy.load(fd)[0]

#print(qc)
print(qc.data[0].params)
print(qc.data[1].params)
print(qc.data[2].params)
print(qc.count_ops())
