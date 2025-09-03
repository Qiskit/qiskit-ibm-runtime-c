# \AggregateApi

All URIs are relative to *https://api.global-search-tagging.cloud.ibm.com*

Method | HTTP request | Description
------------- | ------------- | -------------
[**aggregate**](AggregateApi.md#aggregate) | **POST** /v3/resources/aggregate | Group resources into buckets based on field values



## aggregate

> models::AggregateResult aggregate(aggregate_body, x_request_id, x_correlation_id, timeout, is_deleted, is_reclaimed, is_project_resource)
Group resources into buckets based on field values

Group resources into buckets based on field values, ranges, or other criteria. You can apply query strings if necessary. The query string follows the Lucene syntax, like for the `/v3/resources/search`.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**aggregate_body** | [**AggregateBody**](AggregateBody.md) | It contains the query filters and the terms aggregation instructions. | [required] |
**x_request_id** | Option<**String**> | An alphanumeric string that is used to trace the request. The value  may include ASCII alphanumerics and any of following segment separators: space ( ), comma (,), hyphen, (-), and underscore (_) and may have a length up to 1024 bytes. The value is considered invalid and must be ignored if that value includes any other character or is longer than 1024 bytes or is fewer than 8 characters. If not specified or invalid, it is automatically replaced by a random (version 4) UUID. |  |
**x_correlation_id** | Option<**String**> | An alphanumeric string that is used to trace the request as a part of a larger context: the same value is used for downstream requests and retries of those requests. The value may include ASCII alphanumerics and any of following segment separators: space ( ), comma (,), hyphen, (-), and underscore (_) and may have a length up to 1024 bytes. The value is considered invalid and must be ignored if that value includes any other character or is longer than 1024 bytes or is fewer than 8 characters. If not specified or invalid, it is automatically replaced by a random (version 4) UUID. |  |
**timeout** | Option<**i32**> | A search timeout in milliseconds, bounding the search request to run within the specified time value and bail with the hits accumulated up to that point when expired. Defaults to the system defined timeout. |  |[default to 0]
**is_deleted** | Option<**String**> | Determines if deleted documents should be included in result set or not. Possible values are false (default), true or any. If false, only existing documents are returned; if true, only deleted documents are returned; If any, both existing and deleted documents are returned. (_for administrators only_). |  |[default to false]
**is_reclaimed** | Option<**String**> | Determines if reclaimed documents should be included in result set or not. Possible values are false (default), true or any. If false, only not reclaimed documents are returned; if true, only reclaimed documents are returned; If any, both reclaimed and not reclaimed documents are returned. |  |[default to false]
**is_project_resource** | Option<**String**> | Determines if documents belonging to Project family should be included in result set or not. Possible values are false (default), true or any. If false, documents belonging to all families except Project are returned; if true, only documents belonging to Project family are returned; if any, documents of any family are returned. Only authorized ServiceIds can use this query parameter. |  |[default to false]

### Return type

[**models::AggregateResult**](AggregateResult.md)

### Authorization

[IAM](../README.md#IAM)

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

