# NoiseLearnerInputOptions

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**simulator** | Option<[**models::SamplerV2InputOptionsSimulator**](SamplerV2_input_options_simulator.md)> |  | [optional]
**max_layers_to_learn** | Option<**i32**> | The max number of unique layers to learn | [optional]
**shots_per_randomization** | Option<**i32**> | The total number of shots to use per random learning circuit | [optional]
**num_randomizations** | Option<**i32**> | The number of random circuits to use per learning circuit configuration | [optional]
**layer_pair_depths** | Option<**Vec<i32>**> | The circuit depths (measured in number of pairs) to use in learning experiments | [optional]
**twirling_strategy** | Option<**String**> | The strategy of twirling qubits in identified layers of 2-qubit twirled gates | [optional]
**experimental** | Option<[**serde_json::Value**](.md)> | Experimental options. | [optional]
**support_qiskit** | Option<**bool**> | If True, returns a Qiskit-style output, meant to be parsed using the runtime result decoder, or resort to returning pure JSON results (resulting in larger objects) | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


