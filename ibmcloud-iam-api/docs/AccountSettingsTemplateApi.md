# \AccountSettingsTemplateApi

All URIs are relative to *https://iam.cloud.ibm.com*

Method | HTTP request | Description
------------- | ------------- | -------------
[**commit_account_settings_template**](AccountSettingsTemplateApi.md#commit_account_settings_template) | **POST** /v1/account_settings_templates/{template_id}/versions/{version}/commit | Commit a template version
[**create_account_settings_template**](AccountSettingsTemplateApi.md#create_account_settings_template) | **POST** /v1/account_settings_templates | Create an account settings template
[**create_account_settings_template_version**](AccountSettingsTemplateApi.md#create_account_settings_template_version) | **POST** /v1/account_settings_templates/{template_id}/versions | Create a new version of an account settings template
[**delete_account_settings_template_version**](AccountSettingsTemplateApi.md#delete_account_settings_template_version) | **DELETE** /v1/account_settings_templates/{template_id}/versions/{version} | Delete version of an account settings template
[**delete_all_versions_of_account_settings_template**](AccountSettingsTemplateApi.md#delete_all_versions_of_account_settings_template) | **DELETE** /v1/account_settings_templates/{template_id} | Delete all versions of an account settings template
[**get_account_settings_template_version**](AccountSettingsTemplateApi.md#get_account_settings_template_version) | **GET** /v1/account_settings_templates/{template_id}/versions/{version} | Get version of an account settings template
[**get_latest_account_settings_template_version**](AccountSettingsTemplateApi.md#get_latest_account_settings_template_version) | **GET** /v1/account_settings_templates/{template_id} | Get latest version of an account settings template
[**list_account_settings_templates**](AccountSettingsTemplateApi.md#list_account_settings_templates) | **GET** /v1/account_settings_templates | List account settings templates
[**list_versions_of_account_settings_template**](AccountSettingsTemplateApi.md#list_versions_of_account_settings_template) | **GET** /v1/account_settings_templates/{template_id}/versions | List account settings template versions
[**update_account_settings_template_version**](AccountSettingsTemplateApi.md#update_account_settings_template_version) | **PUT** /v1/account_settings_templates/{template_id}/versions/{version} | Update version of an account settings template



## commit_account_settings_template

> commit_account_settings_template(template_id, version, authorization)
Commit a template version

Commit a specific version of an account settings template in an Enterprise Account. A Template must be committed before being assigned, and once committed, can no longer be modified.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**template_id** | **String** | ID of the account settings template | [required] |
**version** | **String** | Version of the account settings template | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

 (empty response body)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: Not defined

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## create_account_settings_template

> models::AccountSettingsTemplateResponse create_account_settings_template(authorization, account_settings_template_request)
Create an account settings template

Create a new account settings template in an enterprise account.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**account_settings_template_request** | [**AccountSettingsTemplateRequest**](AccountSettingsTemplateRequest.md) | Request to create an account settings template. | [required] |

### Return type

[**models::AccountSettingsTemplateResponse**](AccountSettingsTemplateResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## create_account_settings_template_version

> models::AccountSettingsTemplateResponse create_account_settings_template_version(template_id, authorization, account_settings_template_request)
Create a new version of an account settings template

Create a new version of an account settings template in an Enterprise Account

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**template_id** | **String** | ID of the account settings template | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**account_settings_template_request** | [**AccountSettingsTemplateRequest**](AccountSettingsTemplateRequest.md) | Request to create new version of an account settings template | [required] |

### Return type

[**models::AccountSettingsTemplateResponse**](AccountSettingsTemplateResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## delete_account_settings_template_version

> delete_account_settings_template_version(template_id, version, authorization)
Delete version of an account settings template

Delete a specific version of an account settings template in an Enterprise Account

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**template_id** | **String** | ID of the account settings template | [required] |
**version** | **String** | Version of the account settings template | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

 (empty response body)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: Not defined

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## delete_all_versions_of_account_settings_template

> delete_all_versions_of_account_settings_template(template_id, authorization)
Delete all versions of an account settings template

Delete all versions of an account settings template in an enterprise account. If any version is assigned to child accounts, you must first delete the assignment.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**template_id** | **String** | ID of the account settings template | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

 (empty response body)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: Not defined

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_account_settings_template_version

> models::AccountSettingsTemplateResponse get_account_settings_template_version(template_id, version, authorization, include_history)
Get version of an account settings template

Get a specific version of an account settings template in an Enterprise Account

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**template_id** | **String** | ID of the account settings template | [required] |
**version** | **String** | Version of the account settings template | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**include_history** | Option<**bool**> | Defines if the entity history is included in the response |  |[default to false]

### Return type

[**models::AccountSettingsTemplateResponse**](AccountSettingsTemplateResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_latest_account_settings_template_version

> models::AccountSettingsTemplateResponse get_latest_account_settings_template_version(template_id, authorization, include_history)
Get latest version of an account settings template

Get the latest version of a specific account settings template in an enterprise account.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**template_id** | **String** | ID of the account settings template | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**include_history** | Option<**bool**> | Defines if the entity history is included in the response |  |[default to false]

### Return type

[**models::AccountSettingsTemplateResponse**](AccountSettingsTemplateResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## list_account_settings_templates

> models::AccountSettingsTemplateList list_account_settings_templates(authorization, account_id, limit, pagetoken, sort, order, include_history)
List account settings templates

List account settings templates in an enterprise account

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**account_id** | Option<**String**> | Account ID of the account settings templates to query. This parameter is required unless using a pagetoken. |  |
**limit** | Option<**String**> | Optional size of a single page. |  |[default to 20]
**pagetoken** | Option<**String**> | Optional Prev or Next page token returned from a previous query execution. Default is start with first page. |  |
**sort** | Option<**String**> | Optional sort property. If specified, the returned templated are sorted according to this property |  |[default to created_at]
**order** | Option<**String**> | Optional sort order. |  |[default to asc]
**include_history** | Option<**String**> | Defines if the entity history is included in the response |  |[default to false]

### Return type

[**models::AccountSettingsTemplateList**](AccountSettingsTemplateList.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## list_versions_of_account_settings_template

> models::AccountSettingsTemplateList list_versions_of_account_settings_template(template_id, authorization, limit, pagetoken, sort, order, include_history)
List account settings template versions

List the versions of a specific account settings template in an enterprise account.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**template_id** | **String** | ID of the account settings template | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**limit** | Option<**String**> | Optional size of a single page. |  |[default to 20]
**pagetoken** | Option<**String**> | Optional Prev or Next page token returned from a previous query execution. Default is start with first page. |  |
**sort** | Option<**String**> | Optional sort property. If specified, the returned templated are sorted according to this property |  |[default to created_at]
**order** | Option<**String**> | Optional sort order. |  |[default to asc]
**include_history** | Option<**String**> | Defines if the entity history is included in the response |  |[default to false]

### Return type

[**models::AccountSettingsTemplateList**](AccountSettingsTemplateList.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## update_account_settings_template_version

> models::AccountSettingsTemplateResponse update_account_settings_template_version(if_match, template_id, version, authorization, account_settings_template_request)
Update version of an account settings template

Update a specific version of an account settings template in an Enterprise Account

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**if_match** | **String** | Entity tag of the Template to be updated. Specify the tag that you retrieved when reading the account settings template. This value helps identifying parallel usage of this API. Pass * to indicate to update any version available. This might result in stale updates.  | [required] |
**template_id** | **String** | ID of the account settings template | [required] |
**version** | **String** | Version of the account settings template | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**account_settings_template_request** | [**AccountSettingsTemplateRequest**](AccountSettingsTemplateRequest.md) | Request to create an account settings template. | [required] |

### Return type

[**models::AccountSettingsTemplateResponse**](AccountSettingsTemplateResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

