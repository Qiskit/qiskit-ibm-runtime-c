# \GlobalcatalogCategoriesApi

All URIs are relative to *https://api.global-search-tagging.cloud.ibm.com*

Method | HTTP request | Description
------------- | ------------- | -------------
[**get_catalog_categories**](GlobalcatalogCategoriesApi.md#get_catalog_categories) | **GET** /v3/resources/catalog_categories | Get Globalcatalog categories



## get_catalog_categories

> models::CatalogCategories get_catalog_categories(x_request_id, x_correlation_id, translations_lang)
Get Globalcatalog categories

Collects from Globalcatalog the information about categories, subcategories (children), and services that belong to those categories. 

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**x_request_id** | Option<**String**> | An alphanumeric string that is used to trace the request. The value  may include ASCII alphanumerics and any of following segment separators: space ( ), comma (,), hyphen, (-), and underscore (_) and may have a length up to 1024 bytes. The value is considered invalid and must be ignored if that value includes any other character or is longer than 1024 bytes or is fewer than 8 characters. If not specified or invalid, it is automatically replaced by a random (version 4) UUID. |  |
**x_correlation_id** | Option<**String**> | An alphanumeric string that is used to trace the request as a part of a larger context: the same value is used for downstream requests and retries of those requests. The value may include ASCII alphanumerics and any of following segment separators: space ( ), comma (,), hyphen, (-), and underscore (_) and may have a length up to 1024 bytes. The value is considered invalid and must be ignored if that value includes any other character or is longer than 1024 bytes or is fewer than 8 characters. If not specified or invalid, it is automatically replaced by a random (version 4) UUID. |  |
**translations_lang** | Option<**String**> | The language you want to retrieve the Catalog translations. Default to English if not set. |  |[default to en]

### Return type

[**models::CatalogCategories**](CatalogCategories.md)

### Authorization

[IAM](../README.md#IAM)

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

