# \AccountSettingsApi

All URIs are relative to *https://iam.cloud.ibm.com*

Method | HTTP request | Description
------------- | ------------- | -------------
[**get_account_settings**](AccountSettingsApi.md#get_account_settings) | **GET** /v1/accounts/{account_id}/settings/identity | Get account configurations
[**update_account_settings**](AccountSettingsApi.md#update_account_settings) | **PUT** /v1/accounts/{account_id}/settings/identity | Update account configurations



## get_account_settings

> models::AccountSettingsResponse get_account_settings(account_id, authorization, include_history)
Get account configurations

Returns the details of an account's configuration.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**account_id** | **String** | Unique ID of the account. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**include_history** | Option<**bool**> | Defines if the entity history is included in the response. |  |[default to false]

### Return type

[**models::AccountSettingsResponse**](AccountSettingsResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## update_account_settings

> models::AccountSettingsResponse update_account_settings(authorization, if_match, account_id, account_settings_request)
Update account configurations

Allows a user to configure settings on their account with regards to MFA, MFA excemption list, session lifetimes, access control for creating new identities, and enforcing IP restrictions on token creation.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**if_match** | **String** | Version of the account settings to be updated. Specify the version that you retrieved as entity_tag (ETag header) when reading the account. This value helps identifying parallel usage of this API. Pass * to indicate to update any version available. This might result in stale updates. | [required] |
**account_id** | **String** | The id of the account to update the settings for. | [required] |
**account_settings_request** | [**AccountSettingsRequest**](AccountSettingsRequest.md) | Request to update an account's settings. | [required] |

### Return type

[**models::AccountSettingsResponse**](AccountSettingsResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

