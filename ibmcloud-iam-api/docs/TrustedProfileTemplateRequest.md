# TrustedProfileTemplateRequest

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**account_id** | Option<**String**> | ID of the account where the template resides. | [optional]
**name** | Option<**String**> | The name of the trusted profile template. This is visible only in the enterprise account. Required field when creating a new template. Otherwise this field is optional. If the field is included it will change the name value for all existing versions of the template. | [optional]
**description** | Option<**String**> | The description of the trusted profile template. Describe the template for enterprise account users. | [optional]
**profile** | Option<[**models::TemplateProfileComponentRequest**](TemplateProfileComponentRequest.md)> |  | [optional]
**policy_template_references** | Option<[**Vec<models::PolicyTemplateReference>**](PolicyTemplateReference.md)> | Existing policy templates that you can reference to assign access in the trusted profile component. | [optional]
**action_controls** | Option<[**models::ActionControls**](.md)> |  | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


