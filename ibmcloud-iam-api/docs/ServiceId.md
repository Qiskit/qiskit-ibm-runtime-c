# ServiceId

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**context** | Option<[**models::ResponseContext**](ResponseContext.md)> |  | [optional]
**id** | **String** | Unique identifier of this Service Id. | 
**iam_id** | **String** | Cloud wide identifier for identities of this service ID. | 
**entity_tag** | **String** | Version of the service ID details object. You need to specify this value when updating the service ID to avoid stale updates. | 
**crn** | **String** | Cloud Resource Name of the item. Example Cloud Resource Name: 'crn:v1:bluemix:public:iam-identity:us-south:a/myaccount::serviceid:1234-5678-9012' | 
**locked** | **bool** | The service ID cannot be changed if set to true. | 
**created_at** | **String** | If set contains a date time string of the creation date in ISO format. | 
**modified_at** | **String** | If set contains a date time string of the last modification date in ISO format. | 
**account_id** | **String** | ID of the account the service ID belongs to. | 
**name** | **String** | Name of the Service Id. The name is not checked for uniqueness. Therefore multiple names with the same value can exist. Access is done via the UUID of the Service Id. | 
**description** | Option<**String**> | The optional description of the Service Id. The 'description' property is only available if a description was provided during a create of a Service Id. | [optional]
**unique_instance_crns** | Option<**Vec<String>**> | Optional list of CRNs (string array) which point to the services connected to the service ID. | [optional]
**history** | Option<[**Vec<models::EnityHistoryRecord>**](EnityHistoryRecord.md)> | History of the Service ID. | [optional]
**apikey** | Option<[**models::ApiKey**](ApiKey.md)> |  | [optional]
**activity** | Option<[**models::Activity**](Activity.md)> |  | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


