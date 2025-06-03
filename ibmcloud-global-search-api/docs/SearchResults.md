# SearchResults

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**token** | Option<**String**> | The search token to use on the next call. | [optional]
**more_data** | **bool** | If false, there is no more data to retrieve on the next page. If true, it's possible that there is more data to retrieve on the next page. | 
**items** | [**Vec<models::ResultItem>**](ResultItem.md) | The array of results. Each item represents a resource and contains all visible properties that are associated with it. | 
**filter_error** | **bool** | It is set to true if the result is a partial of an IAM error when user authorization is validated on one or more resources. This field is DEPRECATED and will be removed in future versions of this API | 
**partial_data** | **i32** | Indicates if the result that is set might be partial or not. Value 0 means that the result set is complete. A value greater than 0 means that the result set might be incomplete. Its single bits identify the cause. The first bit means that the error is in the IAM filter. The second bit means that errors are in elasticsearch shards. | 
**offset** | **i32** | Offset parameter specified by the user | 
**limit** | **i32** | Limit parameter specified by the user | 

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


