# TrustedProfile

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**context** | Option<[**models::ResponseContext**](ResponseContext.md)> |  | [optional]
**id** | **String** | the unique identifier of the trusted profile. Example:'Profile-94497d0d-2ac3-41bf-a993-a49d1b14627c' | 
**entity_tag** | **String** | Version of the trusted profile details object. You need to specify this value when updating the trusted profile to avoid stale updates. | 
**crn** | **String** | Cloud Resource Name of the item. Example Cloud Resource Name: 'crn:v1:bluemix:public:iam-identity:us-south:a/myaccount::profile:Profile-94497d0d-2ac3-41bf-a993-a49d1b14627c' | 
**name** | **String** | Name of the trusted profile. The name is checked for uniqueness. Therefore trusted profiles with the same names can not exist in the same account. | 
**description** | Option<**String**> | The optional description of the trusted profile. The 'description' property is only available if a description was provided during a create of a trusted profile. | [optional]
**email** | Option<**String**> | The optional email of the trusted profile. The 'email' property is only available if an email was provided during a create of a trusted profile. | [optional]
**created_at** | Option<**String**> | If set contains a date time string of the creation date in ISO format. | [optional]
**modified_at** | Option<**String**> | If set contains a date time string of the last modification date in ISO format. | [optional]
**iam_id** | **String** | The iam_id of this trusted profile. | 
**account_id** | **String** | ID of the account that this trusted profile belong to. | 
**template_id** | Option<**String**> | ID of the IAM template that was used to create an enterprise-managed trusted profile in your account. When returned, this indicates that the trusted profile is created from and managed by a template in the root enterprise account. | [optional]
**assignment_id** | Option<**String**> | ID of the assignment that was used to create an enterprise-managed trusted profile in your account. When returned, this indicates that the trusted profile is created from and managed by a template in the root enterprise account. | [optional]
**ims_account_id** | Option<**i64**> | IMS acount ID of the trusted profile | [optional]
**ims_user_id** | Option<**i64**> | IMS user ID of the trusted profile | [optional]
**history** | Option<[**Vec<models::EnityHistoryRecord>**](EnityHistoryRecord.md)> | History of the trusted profile. | [optional]
**activity** | Option<[**models::Activity**](Activity.md)> |  | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


