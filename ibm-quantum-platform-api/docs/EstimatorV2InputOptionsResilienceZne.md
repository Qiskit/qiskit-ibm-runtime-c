# EstimatorV2InputOptionsResilienceZne

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**noise_factors** | Option<[**serde_json::Value**](.md)> | A list of floats representing noise factors to use for noise amplification | [optional]
**extrapolator** | Option<[**models::EstimatorV2InputOptionsResilienceZneExtrapolator**](EstimatorV2_input_options_resilience_zne_extrapolator.md)> |  | [optional]
**amplifier** | Option<**String**> | Technique to use for amplifying noise. | [optional]
**extrapolated_noise_factors** | Option<[**serde_json::Value**](.md)> | A list of floats representing noise factors to evaluate the fit extrapolation models at, or the string 'auto' to set them based on the noise factors. | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


