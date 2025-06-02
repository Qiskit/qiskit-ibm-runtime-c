# \ServiceIdOperationsApi

All URIs are relative to *https://iam.cloud.ibm.com*

Method | HTTP request | Description
------------- | ------------- | -------------
[**create_service_id**](ServiceIdOperationsApi.md#create_service_id) | **POST** /v1/serviceids/ | Create a service ID
[**delete_service_id**](ServiceIdOperationsApi.md#delete_service_id) | **DELETE** /v1/serviceids/{id} | Deletes a service ID and associated API keys
[**get_service_id**](ServiceIdOperationsApi.md#get_service_id) | **GET** /v1/serviceids/{id} | Get details of a service ID
[**list_service_ids**](ServiceIdOperationsApi.md#list_service_ids) | **GET** /v1/serviceids/ | List service IDs
[**lock_service_id**](ServiceIdOperationsApi.md#lock_service_id) | **POST** /v1/serviceids/{id}/lock | Lock the service ID
[**unlock_service_id**](ServiceIdOperationsApi.md#unlock_service_id) | **DELETE** /v1/serviceids/{id}/lock | Unlock the service ID
[**update_service_id**](ServiceIdOperationsApi.md#update_service_id) | **PUT** /v1/serviceids/{id} | Update service ID



## create_service_id

> models::ServiceId create_service_id(authorization, create_service_id_request, entity_lock)
Create a service ID

Creates a service ID for an IBM Cloud account. Users can manage user API keys for themself, or service ID API keys for service IDs they have access to.  

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**create_service_id_request** | [**CreateServiceIdRequest**](CreateServiceIdRequest.md) | Request to create a service ID. | [required] |
**entity_lock** | Option<**String**> | Indicates if the service ID is locked for further write operations. False by default. |  |[default to false]

### Return type

[**models::ServiceId**](ServiceId.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## delete_service_id

> delete_service_id(id, authorization)
Deletes a service ID and associated API keys

Deletes a service ID and all API keys associated to it. Before deleting the service ID, all associated API keys are deleted. In case a Delete Conflict (status code 409) a retry of the request may help as the service ID is only deleted if the associated API keys were successfully deleted before. Users can manage user API keys for themself, or service ID API keys for service IDs they have access to.  

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**id** | **String** | Unique ID of the service ID. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

 (empty response body)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: Not defined

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_service_id

> models::ServiceId get_service_id(id, authorization, include_history, include_activity)
Get details of a service ID

Returns the details of a service ID. Users can manage user API keys for themself, or service ID API keys for service IDs they have access to. Note: apikey details are only included in the response when creating a Service ID with an api key.  

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**id** | **String** | Unique ID of the service ID. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**include_history** | Option<**bool**> | Defines if the entity history is included in the response. |  |[default to false]
**include_activity** | Option<**bool**> | Defines if the entity's activity is included in the response. Retrieving activity data is an expensive operation, so only request this when needed. |  |[default to false]

### Return type

[**models::ServiceId**](ServiceId.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## list_service_ids

> models::ServiceIdList list_service_ids(authorization, account_id, name, pagesize, pagetoken, sort, order, include_history, filter)
List service IDs

Returns a list of service IDs. Users can manage user API keys for themself, or service ID API keys for service IDs they have access to. Note: apikey details are only included in the response when creating a Service ID with an api key.  

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**account_id** | Option<**String**> | Account ID of the service ID(s) to query. This parameter is required (unless using a pagetoken). |  |
**name** | Option<**String**> | Name of the service ID(s) to query. Optional.20 items per page. Valid range is 1 to 100. |  |
**pagesize** | Option<**i32**> | Optional size of a single page. Default is 20 items per page. Valid range is 1 to 100. |  |
**pagetoken** | Option<**String**> | Optional Prev or Next page token returned from a previous query execution. Default is start with first page. |  |
**sort** | Option<**String**> | Optional sort property, valid values are name, description, created_at and modified_at. If specified, the items are sorted by the value of this property. |  |
**order** | Option<**String**> | Optional sort order, valid values are asc and desc. Default: asc. |  |[default to asc]
**include_history** | Option<**bool**> | Defines if the entity history is included in the response. |  |[default to false]
**filter** | Option<**String**> | An optional filter query parameter used to refine the results of the search operation. For more information see [Filtering list results](#filter-list-results) section. |  |

### Return type

[**models::ServiceIdList**](ServiceIdList.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## lock_service_id

> lock_service_id(id, authorization)
Lock the service ID

Locks a service ID by ID. Users can manage user API keys for themself, or service ID API keys for service IDs they have access to.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**id** | **String** | Unique ID of the service ID. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

 (empty response body)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: Not defined

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## unlock_service_id

> unlock_service_id(id, authorization)
Unlock the service ID

Unlocks a service ID by ID. Users can manage user API keys for themself, or service ID API keys for service IDs they have access to.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**id** | **String** | Unique ID of the service ID. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

 (empty response body)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: Not defined

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## update_service_id

> models::ServiceId update_service_id(id, if_match, authorization, update_service_id_request)
Update service ID

Updates properties of a service ID. This does NOT affect existing access tokens. Their token content will stay unchanged until the access token is refreshed. To update a service ID, pass the property to be modified. To delete one property's value, pass the property with an empty value \"\".Users can manage user API keys for themself, or service ID API keys for service IDs they have access to. Note: apikey details are only included in the response when creating a Service ID with an apikey.  

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**id** | **String** | Unique ID of the service ID to be updated. | [required] |
**if_match** | **String** | Version of the service ID to be updated. Specify the version that you retrieved as entity_tag (ETag header) when reading the service ID. This value helps identifying parallel usage of this API. Pass * to indicate to update any version available. This might result in stale updates. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**update_service_id_request** | [**UpdateServiceIdRequest**](UpdateServiceIdRequest.md) | Request to update a service ID. | [required] |

### Return type

[**models::ServiceId**](ServiceId.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

