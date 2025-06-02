# \TokenOperationsApi

All URIs are relative to *https://iam.cloud.ibm.com*

Method | HTTP request | Description
------------- | ------------- | -------------
[**get_token_api_key**](TokenOperationsApi.md#get_token_api_key) | **POST** /identity/token#apikey | Create an IAM access token for a user or service ID using an API key
[**get_token_api_key_delegated_refresh_token**](TokenOperationsApi.md#get_token_api_key_delegated_refresh_token) | **POST** /identity/token#apikey-delegated-refresh-token | Create an IAM access token and delegated refresh token for a user or service ID
[**get_token_assume**](TokenOperationsApi.md#get_token_assume) | **POST** /identity/token#assume | Create an IAM access token for a Trusted Profile based on the provided entity. Provided entity can be a identity based token which can be a user token, service id token or a cookie.
[**get_token_cr_token**](TokenOperationsApi.md#get_token_cr_token) | **POST** /identity/token#cr-token | Create an IAM access token for a Trusted Profile based on the provided Compute Resource token
[**get_token_iam_authz**](TokenOperationsApi.md#get_token_iam_authz) | **POST** /identity/token#iam-authz | Create an IAM access token based on an authorization policy
[**get_token_password**](TokenOperationsApi.md#get_token_password) | **POST** /identity/token#password | Create an IAM access token for a user using username / password credentials and an optional account identifier



## get_token_api_key

> models::TokenResponse get_token_api_key(grant_type, apikey, ibm_cloud_tenant)
Create an IAM access token for a user or service ID using an API key

Creates a non-opaque access token for an API key.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**grant_type** | **String** | Grant type for this API call. You must set the grant type to `urn:ibm:params:oauth:grant-type:apikey`. | [required] |
**apikey** | **String** | The value of the api key. | [required] |
**ibm_cloud_tenant** | Option<**String**> | A comma separated list of enterprise ids and/or account ids. If present, an IAM token for the API key can only be created if the account id or enterprise id of the API key is contained in this header. |  |

### Return type

[**models::TokenResponse**](token-response.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/x-www-form-urlencoded
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_token_api_key_delegated_refresh_token

> models::TokenResponse get_token_api_key_delegated_refresh_token(grant_type, apikey, response_type, receiver_client_ids, ibm_cloud_tenant, delegated_refresh_token_expiry)
Create an IAM access token and delegated refresh token for a user or service ID

Creates a non-opaque access token and a delegated refresh token for an API key.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**grant_type** | **String** | Grant type for this API call. You must set the grant type to `urn:ibm:params:oauth:grant-type:apikey`. | [required] |
**apikey** | **String** | The value of the API key. | [required] |
**response_type** | **String** | Either 'delegated_refresh_token' to receive a delegated refresh token only, or 'cloud_iam delegated_refresh_token' to receive both an IAM access token and a delegated refresh token in one API call. | [required] |
**receiver_client_ids** | **String** | A comma separated list of one or more client IDs that will be able to consume the delegated refresh token. The service that accepts a delegated refresh token as API parameter must expose its client ID to allow this API call. The receiver of the delegated refresh token will be able to use the refresh token until it expires. | [required] |
**ibm_cloud_tenant** | Option<**String**> | A comma separated list of enterprise ids and/or account ids. If present, an IAM token for the API key can only be created if the account id or enterprise id of the API key is contained in this header. |  |
**delegated_refresh_token_expiry** | Option<**i32**> | Expiration in seconds until the delegated refresh token must be consumed by the receiver client IDs. After the expiration, no client ID can consume the delegated refresh token, even if the life time of the refresh token inside is still not expired. The default, if not specified, is 518,400 seconds which corresponds to 6 days. |  |

### Return type

[**models::TokenResponse**](token-response.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/x-www-form-urlencoded
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_token_assume

> models::TokenResponse get_token_assume(grant_type, access_token, refresh_token, cookie, profile_id, profile_name, profile_crn, account)
Create an IAM access token for a Trusted Profile based on the provided entity. Provided entity can be a identity based token which can be a user token, service id token or a cookie.

Creates a non-opaque access token for a profile.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**grant_type** | **String** | Grant type for this API call. You must set the grant type to `urn:ibm:params:oauth:grant-type:assume`. | [required] |
**access_token** | Option<**String**> | Pass one of 'access_token', 'refresh_token' or 'cookie' to get a token for the profile. Provided access_token/refresh_token/iam_cookie need to be generated for the user or service id that has trust relationship with the profile. If the profile being assumed must satisfy an MFA requirement for the account, the access_token/refresh_token (...etc) used to assume the profile must meet the same requirement, using the same level MFA or higher. |  |
**refresh_token** | Option<**String**> | see 'access_token' |  |
**cookie** | Option<**String**> | see 'access_token' |  |
**profile_id** | Option<**String**> | Pass one of 'profile_id', 'profile_crn' or 'profile_name' and 'account' to select which profile should be used for this IAM token. If you pass a 'profile_id' or 'profile_crn', then the profile must exist in the same account. If you pass a 'profile_name' then 'account' need to be passed in the request where the profile is looked up based on the account. |  |
**profile_name** | Option<**String**> | see 'profile_id' |  |
**profile_crn** | Option<**String**> | see 'profile_id' |  |
**account** | Option<**String**> | ID of the account the profile belongs to |  |

### Return type

[**models::TokenResponse**](token-response.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/x-www-form-urlencoded
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_token_cr_token

> models::TokenResponse get_token_cr_token(grant_type, cr_token, authorization, ibm_cloud_tenant, profile_id, profile_name, profile_crn)
Create an IAM access token for a Trusted Profile based on the provided Compute Resource token

Creates a non-opaque access token without a refresh token for a Trusted Profile

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**grant_type** | **String** | Grant type for this API call. You must set the grant type to `urn:ibm:params:oauth:grant-type:cr-token`. | [required] |
**cr_token** | **String** | The value of the Compute Resource token. As this is a JWT token, the string can get very long. | [required] |
**authorization** | Option<**String**> | IBM Services can pass in a Basic Authorization Header representing a client id with a secret. For customers, omit this header parameter. To build a valid Basic Authorization Header, concatenate the client id with a colon and the secret, i.e. `client_id:client_secret`. This sequence must be Base64 encoded, and prefixed with `Basic`, so that a valid Basic Authorization Header would be: `Authorization: Basic Y2xpZW50X2lkOmNsaWVudF9zZWNyZXQ=` |  |
**ibm_cloud_tenant** | Option<**String**> | A comma separated list of enterprise ids and/or account ids. If present, an IAM token for the Trusted Profile can only be created if the Trusted Profile is part of one of the account ids or enterprise ids provided in this header. |  |
**profile_id** | Option<**String**> | Pass one of 'profile_id', 'profile_name' or 'profile_crn to select which profile should be used for this IAM token. This call can only succeed if you have also linked the Trusted Profile to the Compute Resource, or you have created a Trust Rule from the Trusted Profile to the Compute Resource. If you pass a 'profile_name', then the profile is looked up based on the account_id of the Compute resource. If you pass a 'profile_id' or 'profile_crn', then the profile must exist in the same account like the Compute Resource. |  |
**profile_name** | Option<**String**> | see 'profile_id' |  |
**profile_crn** | Option<**String**> | see 'profile_id' |  |

### Return type

[**models::TokenResponse**](token-response.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/x-www-form-urlencoded
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_token_iam_authz

> models::TokenResponse get_token_iam_authz(grant_type, access_token, desired_iam_id)
Create an IAM access token based on an authorization policy

Creates a non-opaque access token, if an appropriate authorization policy is in place. This kind of IAM access token is typically used for access between services.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**grant_type** | **String** | Grant type for this API call. You must set the grant type to `urn:ibm:params:oauth:grant-type:iam-authz`. | [required] |
**access_token** | **String** | The IAM access token of the identity that has the appropriate authorization to create an IAM access token for a given resource. | [required] |
**desired_iam_id** | **String** | The IAM ID of the IAM access token identity that should be created. The desired_iam_id identifies a resource identity. The IAM ID consists of the prefix crn- and the CRN of the target identity, e.g. crn-crn:v1:bluemix:public:cloud-object-storage:global:a/59bcbfa6ea2f006b4ed7094c1a08dcdd:1a0ec336-f391-4091-a6fb-5e084a4c56f4::. | [required] |

### Return type

[**models::TokenResponse**](token-response.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/x-www-form-urlencoded
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_token_password

> models::TokenResponse get_token_password(authorization, grant_type, username, password, ibm_cloud_tenant, account)
Create an IAM access token for a user using username / password credentials and an optional account identifier

Creates a non-opaque access token for a username and password. To be able to call IBM Cloud APIs, the token must be made account-specific. For this purpose, also pass the 32 character long identifier for your account in the API call. This API call is possible only for non-federated IBMid users.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**authorization** | **String** | Basic Authorization Header containing a valid client ID and secret. If this header is omitted the request fails with BXNIM0308E: 'No authorization header found'. You can use the client ID and secret that is used by the IBM Cloud CLI: `bx / bx` | [required] |
**grant_type** | **String** | Grant type for this API call. You must set the grant type to `password`. | [required] |
**username** | **String** | The value of the username. | [required] |
**password** | **String** | The value of the password. | [required] |
**ibm_cloud_tenant** | Option<**String**> | A comma separated list of enterprise ids and/or account ids. If present, an IAM token for the username / password / account combination can only be created if the account id matches the passed account or the account is member of the enterprise id in this header. |  |
**account** | Option<**String**> | The 32 character identifier of the account. Specify this parameter to get an account-specific IAM token. IBM Cloud APIs require that IAM tokens are account-specific. |  |

### Return type

[**models::TokenResponse**](token-response.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/x-www-form-urlencoded
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

