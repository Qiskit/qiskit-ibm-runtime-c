# FirstCall

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**query** | **String** | The Lucene-formatted query string. Default to '*' if not set | 
**fields** | Option<**Vec<String>**> | The list of the fields returned by the search. By default, the returned fields are the `account_id`, `name`, `type`, `family`, and `crn`. For all queries, `crn` is always returned. You may set `\"fields\": [\"*\"]` to discover the set of fields available to request. | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


