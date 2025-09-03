# \EffectiveAccountSettingsApi

All URIs are relative to *https://iam.cloud.ibm.com*

Method | HTTP request | Description
------------- | ------------- | -------------
[**get_effective_account_settings**](EffectiveAccountSettingsApi.md#get_effective_account_settings) | **GET** /v1/accounts/{account_id}/effective_settings/identity | Get effective account settings configuration



## get_effective_account_settings

> models::EffectiveAccountSettingsResponse get_effective_account_settings(account_id, authorization, include_history, resolve_user_mfa)
Get effective account settings configuration

Returns effective account settings for given account ID

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**account_id** | **String** | Unique ID of the account. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**include_history** | Option<**bool**> | Defines if the entity history is included in the response. |  |[default to false]
**resolve_user_mfa** | Option<**bool**> | Enrich MFA exemptions with user information. |  |[default to false]

### Return type

[**models::EffectiveAccountSettingsResponse**](EffectiveAccountSettingsResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

