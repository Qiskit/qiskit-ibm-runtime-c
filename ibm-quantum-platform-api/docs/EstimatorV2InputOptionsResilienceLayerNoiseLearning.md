# EstimatorV2InputOptionsResilienceLayerNoiseLearning

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**max_layers_to_learn** | Option<**i32**> | The max number of unique layers to learn | [optional]
**shots_per_randomization** | Option<**i32**> | The total number of shots to use per random learning circuit | [optional]
**num_randomizations** | Option<**i32**> | The number of random circuits to use per learning circuit configuration | [optional]
**layer_pair_depths** | Option<[**serde_json::Value**](.md)> | A list of integers representing the circuit depths (measured in number of pairs) to use in learning experiments | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


