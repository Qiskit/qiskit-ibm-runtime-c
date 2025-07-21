use binrw::{BinResult, BinWrite};
use std::io::Cursor;

use crate::qiskit_circuit;
use crate::qpy_formats;

pub fn generate_qpy_payload(circuit: &qiskit_circuit::Circuit) -> BinResult<Vec<u8>> {
    // Size estimate is "QISKIT" + File Header + Circuit header + CircuitInstruction * num
    // instructions
    //
    // This is an under estimate because it doesn't factor in names, bits, or parameters
    // but it's enough of starting guess to reduce the number of total allocations
    let size_estimate = 6
        + std::mem::size_of::<qpy_formats::FileHeaderV14>()
        + std::mem::size_of::<qpy_formats::QPYFormatV13>()
        + (circuit.num_instructions()
            * std::mem::size_of::<qpy_formats::CircuitInstructionV2Pack>());
    let output = Vec::with_capacity(size_estimate);
    let mut writer = Cursor::new(output);
    qpy_formats::FileHeaderV14 {
        label: *b"QISKIT",
        qpy_version: 14,
        qiskit_major_version: 2,
        qiskit_minor_version: 1,
        qiskit_patch_version: 0,
        num_circuits: 1,
        symbolic_encoding: b'p',
    }
    .write(&mut writer)?;
    qpy_formats::ProgramType { type_key: b'q' }.write(&mut writer)?;
    let empty_json = "{}";
    let circuit_header = qpy_formats::CircuitHeaderV12Pack {
        name_size: 0,
        global_phase_type: b'f',
        global_phase_size: 8,
        num_qubits: circuit.num_qubits(),
        num_clbits: circuit.num_clbits(),
        metadata_size: empty_json.len() as u64,
        num_registers: 1,
        num_instructions: circuit.num_instructions() as u64,
        num_vars: 0,
        circuit_name: Vec::new(),
        global_phase_data: 0_f64.to_be_bytes().to_vec(),
        metadata: empty_json.as_bytes().to_vec(),
        registers: vec![qpy_formats::RegisterV4Pack {
            register_type: b'c',
            standalone: true as u8,
            size: circuit.num_clbits(),
            name_size: 4,
            in_circuit: true as u8,
            name: "meas".as_bytes().to_vec(),
            bit_indices: (0..circuit.num_clbits()).map(|x| x as i64).collect(),
        }]
    };
    let instructions =
        circuit
            .get_circuit_instructions()
            .map(|inst| {
                let out_name = match inst.name.as_str() {
                    "x" => "XGate",
                    "sx" => "SXGate",
                    "cz" => "CZGate",
                    "measure" => "Measure",
                    "reset" => "Reset",
                    "ecr" => "ECRGate",
                    "cx" => "CXGate",
                    "rz" => "RZGate",
                    "id" => "IGate",
                    _ => panic!("Not an ISA circuit"),
                };
                let num_ctrl_qubits = if inst.name.as_str() == "cx" || inst.name.as_str() == "cz" {
                    1
                } else {
                    0
                };
                let bit_data: Vec<qpy_formats::CircuitInstructionArgPack> =
                    inst.qubits
                        .iter()
                        .map(|index| qpy_formats::CircuitInstructionArgPack {
                            bit_type: b'q',
                            index: *index,
                        })
                        .chain(inst.clbits.iter().map(|index| {
                            qpy_formats::CircuitInstructionArgPack {
                                bit_type: b'c',
                                index: *index,
                            }
                        }))
                        .collect();
                let params: Vec<qpy_formats::PackedParam> = inst
                    .params
                    .iter()
                    .map(|param| qpy_formats::PackedParam {
                        type_key: b'f',
                        data_len: 8,
                        data: param.to_le_bytes().to_vec(),
                    })
                    .collect();
                qpy_formats::CircuitInstructionV2Pack {
                    name_size: out_name.len() as u16,
                    label_size: 0,
                    num_parameters: inst.params.len() as u16,
                    num_qargs: inst.qubits.len() as u32,
                    num_cargs: inst.clbits.len() as u32,
                    conditional_key: 0,
                    condition_register_size: 0,
                    condition_value: 0,
                    num_ctrl_qubits,
                    ctrl_state: 1,
                    gate_class_name: out_name.as_bytes().to_vec(),
                    label_raw: Vec::new(),
                    condition_raw: Vec::new(),
                    bit_data,
                    params,
                }
            })
            .collect();

    qpy_formats::QPYFormatV13 {
        header: circuit_header,
        custom_instructions: qpy_formats::CustomCircuitInstructionsPack {
            custom_operations_length: 0,
            custom_instructions: Vec::new(),
        },
        instructions,
        calibration_header: 0_u16.to_be_bytes().to_vec(),
        layout: qpy_formats::LayoutV2Pack {
            exists: 0,
            initial_layout_size: -1,
            input_mapping_size: -1,
            final_layout_size: -1,
            extra_registers_length: 0,
            input_qubit_count: 0,
            extra_registers_data: Vec::new(),
            array_data: Vec::new(),
        },
    }
    .write(&mut writer)?;
    Ok(writer.into_inner())
}
