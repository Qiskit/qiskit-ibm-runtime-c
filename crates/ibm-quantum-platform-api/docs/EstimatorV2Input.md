# EstimatorV2Input

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**pubs** | [**Vec<Vec<serde_json::Value>>**](Vec.md) | Primitive Unit Blocs of data. Each PUB is of the form (Circuit, Observables, Parameters, Precision) where the circuit and observables are required, parameters should be passed only for parametrized circuits, and precision is optional | 
**options** | Option<[**models::EstimatorV2InputOptions**](EstimatorV2_input_options.md)> |  | [optional]
**resilience_level** | Option<**i32**> | How much resilience to build against errors | [optional]
**precision** | Option<**f64**> | Default precision level which applies to all PUBs without precision. This overrides default_precision and default_shots specified in options. | [optional]
**support_qiskit** | Option<**bool**> | If True, returns a qiskit-style output, meant to be parsed using the runtime result decoder, or resort to returning pure JSON results (resulting in larger objects) | [optional]
**version** | Option<**f64**> | For EstimatorV2, version should always be 2 | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


