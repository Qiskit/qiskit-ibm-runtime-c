# EstimatorV2InputOptionsDynamicalDecoupling

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**enable** | Option<**bool**> | Whether to enable dynamical decoupling. | [optional]
**sequence_type** | Option<**String**> | Which dynamical decoupling sequence to use | [optional]
**extra_slack_distribution** | Option<**String**> | Where to put extra timing delays due to rounding issues | [optional]
**scheduling_method** | Option<**String**> | Whether to schedule gates as soon as ('asap') or as late as ('alap') possible | [optional]
**skip_reset_qubits** | Option<**bool**> | Whether to insert DD on idle periods that immediately follow initialized/reset qubits. | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


