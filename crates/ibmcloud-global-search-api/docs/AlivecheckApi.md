# \AlivecheckApi

All URIs are relative to *https://api.global-search-tagging.cloud.ibm.com*

Method | HTTP request | Description
------------- | ------------- | -------------
[**get_alive**](AlivecheckApi.md#get_alive) | **GET** /alive | Get service alive information



## get_alive

> models::AliveResponse get_alive(x_request_id, x_correlation_id)
Get service alive information

alive endpoint

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**x_request_id** | Option<**String**> | An alphanumeric string that is used to trace the request. The value  may include ASCII alphanumerics and any of following segment separators: space ( ), comma (,), hyphen, (-), and underscore (_) and may have a length up to 1024 bytes. The value is considered invalid and must be ignored if that value includes any other character or is longer than 1024 bytes or is fewer than 8 characters. If not specified or invalid, it is automatically replaced by a random (version 4) UUID. |  |
**x_correlation_id** | Option<**String**> | An alphanumeric string that is used to trace the request as a part of a larger context: the same value is used for downstream requests and retries of those requests. The value may include ASCII alphanumerics and any of following segment separators: space ( ), comma (,), hyphen, (-), and underscore (_) and may have a length up to 1024 bytes. The value is considered invalid and must be ignored if that value includes any other character or is longer than 1024 bytes or is fewer than 8 characters. If not specified or invalid, it is automatically replaced by a random (version 4) UUID. |  |

### Return type

[**models::AliveResponse**](AliveResponse.md)

### Authorization

[IAM](../README.md#IAM)

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

