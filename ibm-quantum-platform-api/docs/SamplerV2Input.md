# SamplerV2Input

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**pubs** | [**Vec<models::SamplerV2InputPubsInner>**](SamplerV2_input_pubs_inner.md) | Primitive Unit Blocs of data. Each PUB is of the form (Circuit, Parameters, Shots) where the circuit is required, parameters should be passed only for parametrized circuits, and shots is optional | 
**options** | Option<[**models::SamplerV2InputOptions**](SamplerV2_input_options.md)> |  | [optional]
**shots** | Option<**i32**> | Default number of shots which applies to all PUBs without shots. This overrides default_shots specified in options. | [optional]
**support_qiskit** | Option<**bool**> | If True, returns a Qiskit-style output, meant to be parsed using the runtime result decoder, or resort to returning pure JSON results (resulting in larger objects) | [optional]
**version** | Option<**f64**> | For SamplerV2, version should always be 2 | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


