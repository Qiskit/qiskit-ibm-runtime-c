# EstimatorV2InputOptions

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**seed_estimator** | Option<**i32**> | Seed used to control sampling | [optional]
**default_precision** | Option<**f64**> | The default precision to use if a precision is not specified for a PUB or for this job. | [optional]
**default_shots** | Option<**i32**> | The total number of shots to use per circuit per configuration. If set, this value overrides default_precision. | [optional]
**dynamical_decoupling** | Option<[**models::EstimatorV2InputOptionsDynamicalDecoupling**](EstimatorV2_input_options_dynamical_decoupling.md)> |  | [optional]
**resilience** | Option<[**models::EstimatorV2InputOptionsResilience**](EstimatorV2_input_options_resilience.md)> |  | [optional]
**execution** | Option<[**models::EstimatorV2InputOptionsExecution**](EstimatorV2_input_options_execution.md)> |  | [optional]
**twirling** | Option<[**models::SamplerV2InputOptionsTwirling**](SamplerV2_input_options_twirling.md)> |  | [optional]
**simulator** | Option<[**models::SamplerV2InputOptionsSimulator**](SamplerV2_input_options_simulator.md)> |  | [optional]
**experimental** | Option<[**serde_json::Value**](.md)> | Experimental options. | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


