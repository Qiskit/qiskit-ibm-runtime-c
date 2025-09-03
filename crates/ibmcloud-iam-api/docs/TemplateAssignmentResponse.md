# TemplateAssignmentResponse

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**context** | Option<[**models::ResponseContext**](ResponseContext.md)> |  | [optional]
**id** | **String** | Assignment record Id | 
**account_id** | **String** | Enterprise account Id | 
**template_id** | **String** | Template Id | 
**template_version** | **i64** | Template version | 
**target_type** | **String** | Assignment target type | 
**target** | **String** | Assignment target | 
**status** | **String** | Assignment status | 
**resources** | Option<[**Vec<models::TemplateAssignmentResponseResource>**](TemplateAssignmentResponseResource.md)> | Status breakdown per target account of IAM resources created or errors encountered in attempting to create those IAM resources. IAM resources are only included in the response providing the assignment is not in progress. IAM resources are also only included when getting a single assignment, and excluded by list APIs. | [optional]
**history** | Option<[**Vec<models::EnityHistoryRecord>**](EnityHistoryRecord.md)> | Assignment history | [optional]
**href** | Option<**String**> | Href | [optional]
**created_at** | **String** | Assignment created at | 
**created_by_id** | **String** | IAMid of the identity that created the assignment | 
**last_modified_at** | **String** | Assignment modified at | 
**last_modified_by_id** | **String** | IAMid of the identity that last modified the assignment | 
**entity_tag** | **String** | Entity tag for this assignment record | 

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


