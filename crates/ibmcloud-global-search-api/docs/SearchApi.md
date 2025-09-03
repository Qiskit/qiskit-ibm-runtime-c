# \SearchApi

All URIs are relative to *https://api.global-search-tagging.cloud.ibm.com*

Method | HTTP request | Description
------------- | ------------- | -------------
[**search**](SearchApi.md#search) | **POST** /v3/resources/search | Find instances of resources (v3)
[**v2_search**](SearchApi.md#v2_search) | **POST** /v2/resources/search | Find instances of resources (v2)



## search

> models::ScanResult search(body, x_request_id, x_correlation_id, account_id, limit, timeout, sort, is_deleted, is_reclaimed, is_public, impersonate_user, can_tag, is_project_resource)
Find instances of resources (v3)

Find IAM-enabled resources or storage and network resources that run on classic infrastructure in a specific account ID.  You must use `/v3/resources/search` when you need to fetch more than `10000` resource items. On the first call, the operation returns a live cursor on the data that you must use on all the subsequent calls to get the next batch of results until you get the empty result set.  To filter results, you can apply query strings following the *Lucene* query syntax.  By default, the fields that are returned for every resource are **crn**, **name**, **family**, **type**, and **account_id**. You can specify the subset of the fields you want in your request using the `fields` request body attribute. Set `\"fields\": [\"*\"]` to discover the complete set of fields which are available to request.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**body** | [**SearchRequest**](SearchRequest.md) | It contains the query filters on the first operation call, or the search_cursor on next calls. On subsequent calls, set the `search_cursor` to the value returned by the previous call. After the first, you must set only the `search_cursor`. Any other parameter but the `search_cursor` are ignored. The `search_cursor` encodes all the information that needs to get the next batch of `limit` data. | [required] |
**x_request_id** | Option<**String**> | An alphanumeric string that is used to trace the request. The value  may include ASCII alphanumerics and any of following segment separators: space ( ), comma (,), hyphen, (-), and underscore (_) and may have a length up to 1024 bytes. The value is considered invalid and must be ignored if that value includes any other character or is longer than 1024 bytes or is fewer than 8 characters. If not specified or invalid, it is automatically replaced by a random (version 4) UUID. |  |
**x_correlation_id** | Option<**String**> | An alphanumeric string that is used to trace the request as a part of a larger context: the same value is used for downstream requests and retries of those requests. The value may include ASCII alphanumerics and any of following segment separators: space ( ), comma (,), hyphen, (-), and underscore (_) and may have a length up to 1024 bytes. The value is considered invalid and must be ignored if that value includes any other character or is longer than 1024 bytes or is fewer than 8 characters. If not specified or invalid, it is automatically replaced by a random (version 4) UUID. |  |
**account_id** | Option<**String**> | The account ID to filter resources. |  |
**limit** | Option<**i32**> | The maximum number of hits to return. Defaults to 10. |  |[default to 10]
**timeout** | Option<**i32**> | A search timeout in milliseconds, bounding the search request to run within the specified time value and bail with the hits accumulated up to that point when expired. Defaults to the system defined timeout. |  |[default to 0]
**sort** | Option<[**Vec<String>**](String.md)> | Comma separated properties names that are used for sorting |  |
**is_deleted** | Option<**String**> | Determines if deleted documents should be included in result set or not. Possible values are false (default), true or any. If false, only existing documents are returned; if true, only deleted documents are returned; If any, both existing and deleted documents are returned. (_for administrators only_). |  |[default to false]
**is_reclaimed** | Option<**String**> | Determines if reclaimed documents should be included in result set or not. Possible values are false (default), true or any. If false, only not reclaimed documents are returned; if true, only reclaimed documents are returned; If any, both reclaimed and not reclaimed documents are returned. |  |[default to false]
**is_public** | Option<**String**> | Determines if public resources should be included in result set or not. Possible values are false (default), true or any. If false, do not search public resources; if true, search only public resources; If any, search also public resources. |  |[default to false]
**impersonate_user** | Option<**String**> | The user on whose behalf the search must be performed. Only a GhoST admin can impersonate a user, so be sure you set a GhoST admin IAM token in the Authorization header if you set this parameter. (_for administrators only_). |  |
**can_tag** | Option<**String**> | Determines if the result set must return the resources that the user can tag or the resources that the user can view (only a GhoST admin can use this parameter). If false (default), only resources user can view are returned; if true, only resources that user has permissions for tagging are returned (_for administrators only_). |  |[default to false]
**is_project_resource** | Option<**String**> | Determines if documents belonging to Project family should be included in result set or not. Possible values are false (default), true or any. If false, documents belonging to all families except Project are returned; if true, only documents belonging to Project family are returned; if any, documents of any family are returned. Only authorized ServiceIds can use this query parameter. |  |[default to false]

### Return type

[**models::ScanResult**](ScanResult.md)

### Authorization

[IAM](../README.md#IAM)

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## v2_search

> models::SearchResults v2_search(body, x_request_id, x_correlation_id, account_id, limit, timeout, is_deleted, is_reclaimed, is_public, impersonate_user, can_tag, is_project_resource, offset, sort, x_ims_auth_token, provider)
Find instances of resources (v2)

Find IAM-enabled resources or storage and network resources that run on classic infrastructure in a specific account ID.  To filter results, you can apply query strings following the *Lucene* query syntax.  Only resources that belong to the account ID and that are accessible by the client are returned.  The usage of this API is deprecated for cases when the provider is not specified or `provider=ghost`. `/v3/resources/search` replaces this API for the deprecated cases.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**body** | [**SearchBody**](SearchBody.md) | It contains the query filters, the list of fields to be returned, and the search token (initially set to null or undefined) | [required] |
**x_request_id** | Option<**String**> | An alphanumeric string that is used to trace the request. The value  may include ASCII alphanumerics and any of following segment separators: space ( ), comma (,), hyphen, (-), and underscore (_) and may have a length up to 1024 bytes. The value is considered invalid and must be ignored if that value includes any other character or is longer than 1024 bytes or is fewer than 8 characters. If not specified or invalid, it is automatically replaced by a random (version 4) UUID. |  |
**x_correlation_id** | Option<**String**> | An alphanumeric string that is used to trace the request as a part of a larger context: the same value is used for downstream requests and retries of those requests. The value may include ASCII alphanumerics and any of following segment separators: space ( ), comma (,), hyphen, (-), and underscore (_) and may have a length up to 1024 bytes. The value is considered invalid and must be ignored if that value includes any other character or is longer than 1024 bytes or is fewer than 8 characters. If not specified or invalid, it is automatically replaced by a random (version 4) UUID. |  |
**account_id** | Option<**String**> | The account ID to filter resources. |  |
**limit** | Option<**i32**> | The maximum number of hits to return. Defaults to 10. |  |[default to 10]
**timeout** | Option<**i32**> | A search timeout in milliseconds, bounding the search request to run within the specified time value and bail with the hits accumulated up to that point when expired. Defaults to the system defined timeout. |  |[default to 0]
**is_deleted** | Option<**String**> | Determines if deleted documents should be included in result set or not. Possible values are false (default), true or any. If false, only existing documents are returned; if true, only deleted documents are returned; If any, both existing and deleted documents are returned. (_for administrators only_). |  |[default to false]
**is_reclaimed** | Option<**String**> | Determines if reclaimed documents should be included in result set or not. Possible values are false (default), true or any. If false, only not reclaimed documents are returned; if true, only reclaimed documents are returned; If any, both reclaimed and not reclaimed documents are returned. |  |[default to false]
**is_public** | Option<**String**> | Determines if public resources should be included in result set or not. Possible values are false (default), true or any. If false, do not search public resources; if true, search only public resources; If any, search also public resources. |  |[default to false]
**impersonate_user** | Option<**String**> | The user on whose behalf the search must be performed. Only a GhoST admin can impersonate a user, so be sure you set a GhoST admin IAM token in the Authorization header if you set this parameter. (_for administrators only_). |  |
**can_tag** | Option<**String**> | Determines if the result set must return the resources that the user can tag or the resources that the user can view (only a GhoST admin can use this parameter). If false (default), only resources user can view are returned; if true, only resources that user has permissions for tagging are returned (_for administrators only_). |  |[default to false]
**is_project_resource** | Option<**String**> | Determines if documents belonging to Project family should be included in result set or not. Possible values are false (default), true or any. If false, documents belonging to all families except Project are returned; if true, only documents belonging to Project family are returned; if any, documents of any family are returned. Only authorized ServiceIds can use this query parameter. |  |[default to false]
**offset** | Option<**i32**> | The offset is the index of the item you want to start returning data from. Default is 0. |  |[default to 0]
**sort** | Option<**String**> | Comma separated properties name that is used for sorting |  |
**x_ims_auth_token** | Option<**String**> | The SoftLayer authorization token. You must set it only if you are searching SoftLayer resources, which you do adding `ims` to the search `providers` query parameter. |  |
**provider** | Option<**String**> | Search providers. Supported values are `ghost` and `ims`. If not specified, the defaults to `ghost`. Specify a single provider. |  |[default to ghost]

### Return type

[**models::SearchResults**](SearchResults.md)

### Authorization

[IAM](../README.md#IAM)

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

