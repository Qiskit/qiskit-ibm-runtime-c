# TrustedProfilesList

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**context** | Option<[**models::ResponseContext**](ResponseContext.md)> |  | [optional]
**offset** | Option<**i64**> | The offset of the current page. | [optional]
**limit** | Option<**i64**> | Optional size of a single page. Default is 20 items per page. Valid range is 1 to 100 | [optional]
**first** | Option<**String**> | Link to the first page. | [optional]
**previous** | Option<**String**> | Link to the previous available page. If 'previous' property is not part of the response no previous page is available. | [optional]
**next** | Option<**String**> | Link to the next available page. If 'next' property is not part of the response no next page is available. | [optional]
**profiles** | [**Vec<models::TrustedProfile>**](TrustedProfile.md) | List of trusted profiles | 

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


