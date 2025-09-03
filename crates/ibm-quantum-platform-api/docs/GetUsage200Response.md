# GetUsage200Response

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**instance_id** | **String** |  | 
**plan_id** | **String** |  | 
**usage_period** | Option<[**models::GetUsage200ResponseUsagePeriod**](get_usage_200_response_usage_period.md)> |  | [optional]
**usage_consumed_seconds** | **f64** | Usage consumed in the current usage period | 
**usage_limit_seconds** | Option<**f64**> | The usage limit in seconds during the usage period | [optional]
**usage_allocation_seconds** | Option<**f64**> | The seconds that are allocated for the instance during the usage period | [optional]
**usage_limit_reached** | Option<**bool**> | true if the usage limit is reached. false if not reached. undefined if usage limits do not apply to this instance. | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


