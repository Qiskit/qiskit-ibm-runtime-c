# TrustedProfileTemplateResponse

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**id** | **String** | ID of the the template | 
**version** | **i64** | Version of the the template | 
**account_id** | **String** | ID of the account where the template resides. | 
**name** | **String** | The name of the trusted profile template. This is visible only in the enterprise account. | 
**description** | Option<**String**> | The description of the trusted profile template. Describe the template for enterprise account users. | [optional]
**committed** | Option<**bool**> | Committed flag determines if the template is ready for assignment | [optional]
**profile** | Option<[**models::TemplateProfileComponentResponse**](TemplateProfileComponentResponse.md)> |  | [optional]
**policy_template_references** | Option<[**Vec<models::PolicyTemplateReference>**](PolicyTemplateReference.md)> | Existing policy templates that you can reference to assign access in the trusted profile component. | [optional]
**action_controls** | Option<[**models::ActionControls**](.md)> |  | [optional]
**history** | Option<[**Vec<models::EnityHistoryRecord>**](EnityHistoryRecord.md)> | History of the trusted profile template. | [optional]
**entity_tag** | Option<**String**> | Entity tag for this templateId-version combination | [optional]
**crn** | Option<**String**> | Cloud resource name | [optional]
**created_at** | Option<**String**> | Timestamp of when the template was created | [optional]
**created_by_id** | Option<**String**> | IAMid of the creator | [optional]
**last_modified_at** | Option<**String**> | Timestamp of when the template was last modified | [optional]
**last_modified_by_id** | Option<**String**> | IAMid of the identity that made the latest modification | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


