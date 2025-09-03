# \TrustedProfileTemplateApi

All URIs are relative to *https://iam.cloud.ibm.com*

Method | HTTP request | Description
------------- | ------------- | -------------
[**commit_profile_template**](TrustedProfileTemplateApi.md#commit_profile_template) | **POST** /v1/profile_templates/{template_id}/versions/{version}/commit | Commit a template version
[**create_profile_template**](TrustedProfileTemplateApi.md#create_profile_template) | **POST** /v1/profile_templates | Create a trusted profile template
[**create_profile_template_version**](TrustedProfileTemplateApi.md#create_profile_template_version) | **POST** /v1/profile_templates/{template_id}/versions | Create new version of a trusted profile template
[**delete_all_versions_of_profile_template**](TrustedProfileTemplateApi.md#delete_all_versions_of_profile_template) | **DELETE** /v1/profile_templates/{template_id} | Delete all versions of a trusted profile template
[**delete_profile_template_version**](TrustedProfileTemplateApi.md#delete_profile_template_version) | **DELETE** /v1/profile_templates/{template_id}/versions/{version} | Delete version of trusted profile template
[**get_latest_profile_template_version**](TrustedProfileTemplateApi.md#get_latest_profile_template_version) | **GET** /v1/profile_templates/{template_id} | Get latest version of a trusted profile template
[**get_profile_template_version**](TrustedProfileTemplateApi.md#get_profile_template_version) | **GET** /v1/profile_templates/{template_id}/versions/{version} | Get version of trusted profile template
[**list_profile_templates**](TrustedProfileTemplateApi.md#list_profile_templates) | **GET** /v1/profile_templates | List trusted profile templates
[**list_versions_of_profile_template**](TrustedProfileTemplateApi.md#list_versions_of_profile_template) | **GET** /v1/profile_templates/{template_id}/versions | List trusted profile template versions
[**update_profile_template_version**](TrustedProfileTemplateApi.md#update_profile_template_version) | **PUT** /v1/profile_templates/{template_id}/versions/{version} | Update version of trusted profile template



## commit_profile_template

> commit_profile_template(template_id, version, authorization)
Commit a template version

Commit a specific version of a trusted profile template in an enterprise account. You must commit a template before you can assign it to child accounts. Once a template is committed, you can no longer modify the template.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**template_id** | **String** | ID of the trusted profile template | [required] |
**version** | **String** | Version of the Profile Template | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

 (empty response body)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: Not defined

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## create_profile_template

> models::TrustedProfileTemplateResponse create_profile_template(authorization, trusted_profile_template_request)
Create a trusted profile template

Create a new trusted profile template in an enterprise account.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**trusted_profile_template_request** | [**TrustedProfileTemplateRequest**](TrustedProfileTemplateRequest.md) | Request to create a trusted profile template. | [required] |

### Return type

[**models::TrustedProfileTemplateResponse**](TrustedProfileTemplateResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## create_profile_template_version

> models::TrustedProfileTemplateResponse create_profile_template_version(template_id, authorization, trusted_profile_template_request)
Create new version of a trusted profile template

Create a new version of a trusted profile template in an enterprise account.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**template_id** | **String** | ID of the trusted profile template | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**trusted_profile_template_request** | [**TrustedProfileTemplateRequest**](TrustedProfileTemplateRequest.md) | Request to create new version of a Trusted Profile Template | [required] |

### Return type

[**models::TrustedProfileTemplateResponse**](TrustedProfileTemplateResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## delete_all_versions_of_profile_template

> delete_all_versions_of_profile_template(template_id, authorization)
Delete all versions of a trusted profile template

Delete all versions of a trusted profile template in an enterprise account. If any version is assigned to child accounts, you must first delete the assignment.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**template_id** | **String** | ID of the trusted profile template | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

 (empty response body)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: Not defined

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## delete_profile_template_version

> delete_profile_template_version(template_id, version, authorization)
Delete version of trusted profile template

Delete a specific version of a trusted profile template in an enterprise account. If the version is assigned to child accounts, you must first delete the assignment.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**template_id** | **String** | ID of the trusted profile template | [required] |
**version** | **String** | Version of the Profile Template | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

 (empty response body)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: Not defined

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_latest_profile_template_version

> models::TrustedProfileTemplateResponse get_latest_profile_template_version(template_id, authorization, include_history)
Get latest version of a trusted profile template

Get the latest version of a trusted profile template in an enterprise account.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**template_id** | **String** | ID of the trusted profile template | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**include_history** | Option<**bool**> | Defines if the entity history is included in the response |  |[default to false]

### Return type

[**models::TrustedProfileTemplateResponse**](TrustedProfileTemplateResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_profile_template_version

> models::TrustedProfileTemplateResponse get_profile_template_version(template_id, version, authorization, include_history)
Get version of trusted profile template

Get a specific version of a trusted profile template in an enterprise account.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**template_id** | **String** | ID of the trusted profile template | [required] |
**version** | **String** | Version of the Profile Template | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**include_history** | Option<**bool**> | Defines if the entity history is included in the response |  |[default to false]

### Return type

[**models::TrustedProfileTemplateResponse**](TrustedProfileTemplateResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## list_profile_templates

> models::TrustedProfileTemplateList list_profile_templates(authorization, account_id, limit, pagetoken, sort, order, include_history)
List trusted profile templates

List the trusted profile templates in an enterprise account.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**account_id** | Option<**String**> | Account ID of the trusted profile templates to query. This parameter is required unless using a pagetoken. |  |
**limit** | Option<**String**> | Optional size of a single page. |  |[default to 20]
**pagetoken** | Option<**String**> | Optional Prev or Next page token returned from a previous query execution. Default is start with first page. |  |
**sort** | Option<**String**> | Optional sort property. If specified, the returned templates are sorted according to this property. |  |[default to created_at]
**order** | Option<**String**> | Optional sort order. |  |[default to asc]
**include_history** | Option<**String**> | Defines if the entity history is included in the response. |  |[default to false]

### Return type

[**models::TrustedProfileTemplateList**](TrustedProfileTemplateList.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## list_versions_of_profile_template

> models::TrustedProfileTemplateList list_versions_of_profile_template(template_id, authorization, limit, pagetoken, sort, order, include_history)
List trusted profile template versions

List the versions of a trusted profile template in an enterprise account.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**template_id** | **String** | ID of the trusted profile template | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**limit** | Option<**String**> | Optional size of a single page. |  |[default to 20]
**pagetoken** | Option<**String**> | Optional Prev or Next page token returned from a previous query execution. Default is start with first page. |  |
**sort** | Option<**String**> | Optional sort property. If specified, the returned templated are sorted according to this property |  |[default to created_at]
**order** | Option<**String**> | Optional sort order. |  |[default to asc]
**include_history** | Option<**String**> | Defines if the entity history is included in the response |  |[default to false]

### Return type

[**models::TrustedProfileTemplateList**](TrustedProfileTemplateList.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## update_profile_template_version

> models::TrustedProfileTemplateResponse update_profile_template_version(if_match, template_id, version, authorization, trusted_profile_template_request)
Update version of trusted profile template

Update a specific version of a trusted profile template in an enterprise account.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**if_match** | **String** | Entity tag of the Template to be updated. Specify the tag that you retrieved when reading the Profile Template. This value helps identifying parallel usage of this API. Pass * to indicate to update any version available. This might result in stale updates.  | [required] |
**template_id** | **String** | ID of the trusted profile template | [required] |
**version** | **String** | Version of the Profile Template | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**trusted_profile_template_request** | [**TrustedProfileTemplateRequest**](TrustedProfileTemplateRequest.md) | Request to create a trusted profile template. | [required] |

### Return type

[**models::TrustedProfileTemplateResponse**](TrustedProfileTemplateResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

