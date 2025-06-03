# ScanResult

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**search_cursor** | Option<**String**> | The search cursor to use on all calls after the first one. | [optional]
**limit** | **i32** | Value of the limit parameter specified by the user | 
**items** | [**Vec<models::ResultItem>**](ResultItem.md) | The array of results. Each item represents a resource. For each resource, the requested `fields` are returned. If you did not set the `fields` request body parameter, then the `account_id`, `name`, `type`, `family`, and `crn` are returned. An empty array signals the end of the result set, which means there are no more results to fetch | 

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


