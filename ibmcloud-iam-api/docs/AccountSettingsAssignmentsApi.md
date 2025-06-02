# \AccountSettingsAssignmentsApi

All URIs are relative to *https://iam.cloud.ibm.com*

Method | HTTP request | Description
------------- | ------------- | -------------
[**create_account_settings_assignment**](AccountSettingsAssignmentsApi.md#create_account_settings_assignment) | **POST** /v1/account_settings_assignments/ | Create assignment
[**delete_account_settings_assignment**](AccountSettingsAssignmentsApi.md#delete_account_settings_assignment) | **DELETE** /v1/account_settings_assignments/{assignment_id} | Delete assignment
[**get_account_settings_assignment**](AccountSettingsAssignmentsApi.md#get_account_settings_assignment) | **GET** /v1/account_settings_assignments/{assignment_id} | Get assignment
[**list_account_settings_assignments**](AccountSettingsAssignmentsApi.md#list_account_settings_assignments) | **GET** /v1/account_settings_assignments/ | List assignments
[**update_account_settings_assignment**](AccountSettingsAssignmentsApi.md#update_account_settings_assignment) | **PATCH** /v1/account_settings_assignments/{assignment_id} | Update assignment



## create_account_settings_assignment

> models::TemplateAssignmentResponse create_account_settings_assignment(create_template_assignment_request, authorization)
Create assignment

Create an assigment for an account settings template.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**create_template_assignment_request** | [**CreateTemplateAssignmentRequest**](CreateTemplateAssignmentRequest.md) | Body parameters to create an account settings template Assignment | [required] |
**authorization** | Option<**String**> | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. |  |

### Return type

[**models::TemplateAssignmentResponse**](TemplateAssignmentResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## delete_account_settings_assignment

> models::ExceptionResponse delete_account_settings_assignment(assignment_id, authorization)
Delete assignment

Delete an account settings template assignment. This removes any IAM resources created by this assignment in child accounts.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**assignment_id** | **String** | ID of the Assignment Record | [required] |
**authorization** | Option<**String**> | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. |  |

### Return type

[**models::ExceptionResponse**](ExceptionResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_account_settings_assignment

> models::TemplateAssignmentResponse get_account_settings_assignment(assignment_id, include_history, authorization)
Get assignment

Get an assigment for an account settings template.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**assignment_id** | **String** | ID of the Assignment Record | [required] |
**include_history** | Option<**bool**> | Defines if the entity history is included in the response |  |[default to false]
**authorization** | Option<**String**> | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. |  |

### Return type

[**models::TemplateAssignmentResponse**](TemplateAssignmentResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## list_account_settings_assignments

> models::TemplateAssignmentListResponse list_account_settings_assignments(account_id, template_id, template_version, target, target_type, limit, pagetoken, sort, order, include_history, authorization)
List assignments

List account settings assignments.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**account_id** | Option<**String**> | Account ID of the Assignments to query. This parameter is required unless using a pagetoken. |  |
**template_id** | Option<**String**> | Filter results by Template Id |  |
**template_version** | Option<**String**> | Filter results Template Version |  |
**target** | Option<**String**> | Filter results by the assignment target |  |
**target_type** | Option<**String**> | Filter results by the assignment's target type |  |
**limit** | Option<**i32**> | Optional size of a single page. Default is 20 items per page. Valid range is 1 to 100 |  |[default to 20]
**pagetoken** | Option<**String**> | Optional Prev or Next page token returned from a previous query execution. Default is start with first page. |  |
**sort** | Option<**String**> | If specified, the items are sorted by the value of this property |  |[default to created_at]
**order** | Option<**String**> | Sort order |  |[default to asc]
**include_history** | Option<**bool**> | Defines if the entity history is included in the response |  |[default to false]
**authorization** | Option<**String**> | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. |  |

### Return type

[**models::TemplateAssignmentListResponse**](TemplateAssignmentListResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## update_account_settings_assignment

> models::TemplateAssignmentResponse update_account_settings_assignment(assignment_id, if_match, update_template_assignment_request, authorization)
Update assignment

Update an account settings assignment. Call this method to retry failed assignments or migrate the settings in child accounts to a new version.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**assignment_id** | **String** | ID of the Assignment Record | [required] |
**if_match** | **String** | Version of the assignment to be updated. Specify the version that you retrieved when reading the assignment. This value  helps identifying parallel usage of this API. Pass * to indicate to update any version available. This might result in stale updates.  | [required] |
**update_template_assignment_request** | [**UpdateTemplateAssignmentRequest**](UpdateTemplateAssignmentRequest.md) | Request to update an assignment | [required] |
**authorization** | Option<**String**> | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. |  |

### Return type

[**models::TemplateAssignmentResponse**](TemplateAssignmentResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

