# ApiKeyList

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**context** | Option<[**models::ResponseContext**](ResponseContext.md)> |  | [optional]
**offset** | Option<**i64**> | The offset of the current page. | [optional]
**limit** | Option<**i64**> | Optional size of a single page. Default is 20 items per page. Valid range is 1 to 100 | [optional]
**first** | Option<**String**> | Link to the first page. | [optional]
**previous** | Option<**String**> | Link to the previous available page. If 'previous' property is not part of the response no previous page is available. | [optional]
**next** | Option<**String**> | Link to the next available page. If 'next' property is not part of the response no next page is available. | [optional]
**apikeys** | [**Vec<models::ApiKey>**](ApiKey.md) | List of API keys based on the query paramters and the page size. The apikeys array is always part of the response but might be empty depending on the query parameters values provided. | 

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


