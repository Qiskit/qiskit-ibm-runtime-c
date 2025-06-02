# TrustedProfileTemplateList

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**context** | Option<[**models::ResponseContext**](ResponseContext.md)> |  | [optional]
**offset** | Option<**i64**> | The offset of the current page. | [optional]
**limit** | Option<**i64**> | Optional size of a single page. | [optional][default to 20]
**first** | Option<**String**> | Link to the first page. | [optional]
**previous** | Option<**String**> | Link to the previous available page. If 'previous' property is not part of the response no previous page is available. | [optional]
**next** | Option<**String**> | Link to the next available page. If 'next' property is not part of the response no next page is available. | [optional]
**profile_templates** | [**Vec<models::TrustedProfileTemplateResponse>**](TrustedProfileTemplateResponse.md) | List of Profile Templates based on the query paramters and the page size. The profile_templates array is always part of the response but might be empty depending on the query parameter values provided. | 

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


