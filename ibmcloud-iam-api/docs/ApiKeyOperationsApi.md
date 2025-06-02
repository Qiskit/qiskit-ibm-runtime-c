# \ApiKeyOperationsApi

All URIs are relative to *https://iam.cloud.ibm.com*

Method | HTTP request | Description
------------- | ------------- | -------------
[**create_api_key**](ApiKeyOperationsApi.md#create_api_key) | **POST** /v1/apikeys | Create an API key
[**delete_api_key**](ApiKeyOperationsApi.md#delete_api_key) | **DELETE** /v1/apikeys/{id} | Deletes an API key
[**disable_api_key**](ApiKeyOperationsApi.md#disable_api_key) | **POST** /v1/apikeys/{id}/disable | Disable the API key
[**enable_api_key**](ApiKeyOperationsApi.md#enable_api_key) | **DELETE** /v1/apikeys/{id}/disable | Enable the API key
[**get_api_key**](ApiKeyOperationsApi.md#get_api_key) | **GET** /v1/apikeys/{id} | Get details of an API key
[**get_api_keys_details**](ApiKeyOperationsApi.md#get_api_keys_details) | **GET** /v1/apikeys/details | Get details of an API key by its value.
[**list_api_keys**](ApiKeyOperationsApi.md#list_api_keys) | **GET** /v1/apikeys | Get API keys for a given service or user IAM ID and account ID
[**lock_api_key**](ApiKeyOperationsApi.md#lock_api_key) | **POST** /v1/apikeys/{id}/lock | Lock the API key
[**unlock_api_key**](ApiKeyOperationsApi.md#unlock_api_key) | **DELETE** /v1/apikeys/{id}/lock | Unlock the API key
[**update_api_key**](ApiKeyOperationsApi.md#update_api_key) | **PUT** /v1/apikeys/{id} | Updates an API key



## create_api_key

> models::ApiKey create_api_key(authorization, create_api_key_request, entity_lock, entity_disable)
Create an API key

Creates an API key for a UserID or service ID. Users can manage user API keys for themself, or service ID API keys for service IDs they have access to.  

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**create_api_key_request** | [**CreateApiKeyRequest**](CreateApiKeyRequest.md) | Request to create an API key. | [required] |
**entity_lock** | Option<**String**> | Indicates if the API key is locked for further write operations. False by default. |  |[default to false]
**entity_disable** | Option<**String**> | Indicates if the API key is disabled. False by default. |  |[default to false]

### Return type

[**models::ApiKey**](ApiKey.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## delete_api_key

> delete_api_key(id, authorization)
Deletes an API key

Deletes an API key. Existing tokens will remain valid until expired. Users can manage user API keys for themself, or service ID API keys for service IDs they have access to.  

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**id** | **String** | Unique ID of the API key. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

 (empty response body)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: Not defined

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## disable_api_key

> disable_api_key(id, authorization)
Disable the API key

Disable an API key. Users can manage user API keys for themself, or service ID API keys for service IDs they have access to.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**id** | **String** | Unique ID of the API key. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

 (empty response body)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: Not defined

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## enable_api_key

> enable_api_key(id, authorization)
Enable the API key

Enable an API key. Users can manage user API keys for themself, or service ID API keys for service IDs they have access to.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**id** | **String** | Unique ID of the API key. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

 (empty response body)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: Not defined

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_api_key

> models::ApiKey get_api_key(id, authorization, include_history, include_activity)
Get details of an API key

Returns the details of an API key. Users can manage user API keys for themself, or service ID API keys for service IDs they have access to.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**id** | **String** | Unique ID of the API key. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**include_history** | Option<**bool**> | Defines if the entity history is included in the response. |  |[default to false]
**include_activity** | Option<**bool**> | Defines if the entity's activity is included in the response. Retrieving activity data is an expensive operation, so only request this when needed. |  |[default to false]

### Return type

[**models::ApiKey**](ApiKey.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_api_keys_details

> models::ApiKey get_api_keys_details(authorization, iam_api_key, include_history)
Get details of an API key by its value.

Returns the details of an API key by its value. Users can manage user API keys for themself, or service ID API keys for service IDs they have access to.  

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**iam_api_key** | Option<**String**> | API key value. |  |
**include_history** | Option<**bool**> | Defines if the entity history is included in the response. |  |[default to false]

### Return type

[**models::ApiKey**](ApiKey.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## list_api_keys

> models::ApiKeyList list_api_keys(authorization, account_id, iam_id, pagesize, pagetoken, scope, r#type, sort, order, include_history, filter)
Get API keys for a given service or user IAM ID and account ID

Returns the list of API key details for a given service or user IAM ID and account ID. Users can manage user API keys for themself, or service ID API keys for service IDs they have access to.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**account_id** | Option<**String**> | Account ID of the API keys to query. If a service IAM ID is specified in iam_id then account_id must match the account of the IAM ID. If a user IAM ID is specified in iam_id then then account_id must match the account of the Authorization token. |  |
**iam_id** | Option<**String**> | IAM ID of the API keys to be queried. The IAM ID may be that of a user or a service. For a user IAM ID iam_id must match the Authorization token. |  |
**pagesize** | Option<**i32**> | Optional size of a single page. Default is 20 items per page. Valid range is 1 to 100. |  |
**pagetoken** | Option<**String**> | Optional Prev or Next page token returned from a previous query execution. Default is start with first page. |  |
**scope** | Option<**String**> | Optional parameter to define the scope of the queried API keys. Can be 'entity' (default) or 'account'. |  |[default to entity]
**r#type** | Option<**String**> | Optional parameter to filter the type of the queried API keys. Can be 'user' or 'serviceid'. |  |
**sort** | Option<**String**> | Optional sort property, valid values are name, description, created_at and created_by. If specified, the items are sorted by the value of this property. |  |
**order** | Option<**String**> | Optional sort order, valid values are asc and desc. Default: asc. |  |[default to asc]
**include_history** | Option<**bool**> | Defines if the entity history is included in the response. |  |[default to false]
**filter** | Option<**String**> | An optional filter query parameter used to refine the results of the search operation. For more information see [Filtering list results](#filter-list-results) section. |  |

### Return type

[**models::ApiKeyList**](ApiKeyList.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## lock_api_key

> lock_api_key(id, authorization)
Lock the API key

Locks an API key by ID. Users can manage user API keys for themself, or service ID API keys for service IDs they have access to.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**id** | **String** | Unique ID of the API key. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

 (empty response body)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: Not defined

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## unlock_api_key

> unlock_api_key(id, authorization)
Unlock the API key

Unlocks an API key by ID. Users can manage user API keys for themself, or service ID API keys for service IDs they have access to.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**id** | **String** | Unique ID of the API key. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

 (empty response body)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: Not defined

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## update_api_key

> models::ApiKey update_api_key(id, if_match, authorization, update_api_key_request)
Updates an API key

Updates properties of an API key. This does NOT affect existing access tokens. Their token content will stay unchanged until the access token is refreshed. To update an API key, pass the property to be modified. To delete one property's value, pass the property with an empty value \"\". Users can manage user API keys for themself, or service ID API keys for service IDs they have access to.  

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**id** | **String** | Unique ID of the API key to be updated. | [required] |
**if_match** | **String** | Version of the API key to be updated. Specify the version that you retrieved when reading the API key. This value helps identifying parallel usage of this API. Pass * to indicate to update any version available. This might result in stale updates.  | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**update_api_key_request** | [**UpdateApiKeyRequest**](UpdateApiKeyRequest.md) | Request to update an API key. | [required] |

### Return type

[**models::ApiKey**](ApiKey.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

