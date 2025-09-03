# EstimatorV2InputOptionsResilience

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**measure_mitigation** | Option<**bool**> | Whether to enable measurement error mitigation method | [optional]
**measure_noise_learning** | Option<[**models::EstimatorV2InputOptionsResilienceMeasureNoiseLearning**](EstimatorV2_input_options_resilience_measure_noise_learning.md)> |  | [optional]
**zne_mitigation** | Option<**bool**> | Whether to turn on Zero Noise Extrapolation error mitigation method | [optional]
**zne** | Option<[**models::EstimatorV2InputOptionsResilienceZne**](EstimatorV2_input_options_resilience_zne.md)> |  | [optional]
**pec_mitigation** | Option<**bool**> | Whether to turn on Probabilistic Error Cancellation error mitigation method | [optional]
**pec** | Option<[**models::EstimatorV2InputOptionsResiliencePec**](EstimatorV2_input_options_resilience_pec.md)> |  | [optional]
**layer_noise_learning** | Option<[**models::EstimatorV2InputOptionsResilienceLayerNoiseLearning**](EstimatorV2_input_options_resilience_layer_noise_learning.md)> |  | [optional]
**layer_noise_model** | Option<[**serde_json::Value**](.md)> | A NoiseLearnerResult or a sequence of LayerError objects, or ``None`` to invoke a noise-learning stage when necessary. | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


