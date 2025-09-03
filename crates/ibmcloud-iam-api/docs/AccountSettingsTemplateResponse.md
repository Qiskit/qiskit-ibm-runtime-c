# AccountSettingsTemplateResponse

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**id** | **String** | ID of the the template | 
**version** | **i64** | Version of the the template | 
**account_id** | **String** | ID of the account where the template resides. | 
**name** | **String** | The name of the trusted profile template. This is visible only in the enterprise account. | 
**description** | Option<**String**> | The description of the trusted profile template. Describe the template for enterprise account users. | [optional]
**committed** | **bool** | Committed flag determines if the template is ready for assignment | 
**account_settings** | [**models::AccountSettingsComponent**](AccountSettingsComponent.md) |  | 
**history** | Option<[**Vec<models::EnityHistoryRecord>**](EnityHistoryRecord.md)> | History of the Template. | [optional]
**entity_tag** | **String** | Entity tag for this templateId-version combination | 
**crn** | **String** | Cloud resource name | 
**created_at** | Option<**String**> | Template Created At | [optional]
**created_by_id** | Option<**String**> | IAMid of the creator | [optional]
**last_modified_at** | Option<**String**> | Template last modified at | [optional]
**last_modified_by_id** | Option<**String**> | IAMid of the identity that made the latest modification | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


