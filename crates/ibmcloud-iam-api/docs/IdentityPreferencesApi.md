# \IdentityPreferencesApi

All URIs are relative to *https://iam.cloud.ibm.com*

Method | HTTP request | Description
------------- | ------------- | -------------
[**delete_preferences_on_scope_account**](IdentityPreferencesApi.md#delete_preferences_on_scope_account) | **DELETE** /v1/preferences/accounts/{account_id}/identities/{iam_id}/{service}/{preference_id} | Delete Identity Preference on scope account
[**get_all_preferences_on_scope_account**](IdentityPreferencesApi.md#get_all_preferences_on_scope_account) | **GET** /v1/preferences/accounts/{account_id}/identities/{iam_id} | Get all Identity Preferences for one account
[**get_preferences_on_scope_account**](IdentityPreferencesApi.md#get_preferences_on_scope_account) | **GET** /v1/preferences/accounts/{account_id}/identities/{iam_id}/{service}/{preference_id} | Get Identity Preference on scope account
[**update_preference_on_scope_account**](IdentityPreferencesApi.md#update_preference_on_scope_account) | **PUT** /v1/preferences/accounts/{account_id}/identities/{iam_id}/{service}/{preference_id} | Update Identity Preference on scope account



## delete_preferences_on_scope_account

> models::ExceptionResponse delete_preferences_on_scope_account(account_id, iam_id, service, preference_id, authorization)
Delete Identity Preference on scope account

Delete one Identity Preference on scope 'account'.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**account_id** | **String** | Account id to delete preference for | [required] |
**iam_id** | **String** | IAM id to delete the preference for | [required] |
**service** | **String** | Service of the preference to be deleted | [required] |
**preference_id** | **String** | Identifier of preference to be deleted | [required] |
**authorization** | Option<**String**> | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. |  |

### Return type

[**models::ExceptionResponse**](ExceptionResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_all_preferences_on_scope_account

> models::IdentityPreferencesResponse get_all_preferences_on_scope_account(account_id, iam_id, authorization)
Get all Identity Preferences for one account

Get all Identity Preferences for one account / user combination.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**account_id** | **String** | Account id to get preferences for | [required] |
**iam_id** | **String** | IAM id to get the preferences for | [required] |
**authorization** | Option<**String**> | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. |  |

### Return type

[**models::IdentityPreferencesResponse**](IdentityPreferencesResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_preferences_on_scope_account

> models::IdentityPreferenceResponse get_preferences_on_scope_account(account_id, iam_id, service, preference_id, authorization)
Get Identity Preference on scope account

Get one Identity Preference on scope 'account'.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**account_id** | **String** | Account id to get preference for | [required] |
**iam_id** | **String** | IAM id to get the preference for | [required] |
**service** | **String** | Service of the preference to be fetched | [required] |
**preference_id** | **String** | Identifier of preference to be fetched | [required] |
**authorization** | Option<**String**> | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. |  |

### Return type

[**models::IdentityPreferenceResponse**](IdentityPreferenceResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## update_preference_on_scope_account

> models::IdentityPreferenceResponse update_preference_on_scope_account(account_id, iam_id, service, preference_id, update_preference_request, authorization)
Update Identity Preference on scope account

Update one Identity Preference on scope 'account'. supported preferences:   The following preferences are storing values for identities inside an account,    i.e. for each account that an identity is member of, the value stored might be different.    This means, users who might be member of multiple accounts can have multiple preferences, one per account.    Identities like Service Ids or Trusted Profiles can only exist in one account,    therefore they can only have one preference inside their related account.   preference: console/landing_page     service: console     preferenceId: landing_page     supportedIdentityType: Trusted Profiles, Users     type: string     validation: valid URL (without host part), e.g. /billing or /iam   preference: console/global_left_navigation     service: console     preferenceId: global_left_navigation     supportedIdentityType: Trusted Profiles, Users     type: list of strings     validation: each entry in the list of strings must match the identifier of one navigation entry in the console

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**account_id** | **String** | Account id to update preference for | [required] |
**iam_id** | **String** | IAM id to update the preference for | [required] |
**service** | **String** | Service of the preference to be updated | [required] |
**preference_id** | **String** | Identifier of preference to be updated | [required] |
**update_preference_request** | [**UpdatePreferenceRequest**](UpdatePreferenceRequest.md) | Request to create one identity preference on scope 'acount'. | [required] |
**authorization** | Option<**String**> | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. |  |

### Return type

[**models::IdentityPreferenceResponse**](IdentityPreferenceResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

