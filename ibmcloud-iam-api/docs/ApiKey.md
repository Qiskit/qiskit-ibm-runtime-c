# ApiKey

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**context** | Option<[**models::ResponseContext**](ResponseContext.md)> |  | [optional]
**id** | **String** | Unique identifier of this API Key. | 
**entity_tag** | Option<**String**> | Version of the API Key details object. You need to specify this value when updating the API key to avoid stale updates. | [optional]
**crn** | **String** | Cloud Resource Name of the item. Example Cloud Resource Name: 'crn:v1:bluemix:public:iam-identity:us-south:a/myaccount::apikey:1234-9012-5678' | 
**locked** | **bool** | The API key cannot be changed if set to true. | 
**disabled** | Option<**bool**> | Defines if API key is disabled, API key cannot be used if 'disabled' is set to true. | [optional]
**created_at** | Option<**String**> | If set contains a date time string of the creation date in ISO format. | [optional]
**created_by** | **String** | IAM ID of the user or service which created the API key. | 
**modified_at** | Option<**String**> | If set contains a date time string of the last modification date in ISO format. | [optional]
**name** | **String** | Name of the API key. The name is not checked for uniqueness. Therefore multiple names with the same value can exist. Access is done via the UUID of the API key. | 
**support_sessions** | Option<**bool**> | Defines whether you can manage CLI login sessions for the API key. When `true`, sessions are created and can be reviewed or revoked. When `false`, no sessions are tracked. To block access, delete or rotate the API key. Available only for user API keys. | [optional]
**action_when_leaked** | Option<**String**> | Defines the action to take when API key is leaked, valid values are 'none', 'disable' and 'delete'. | [optional]
**description** | Option<**String**> | The optional description of the API key. The 'description' property is only available if a description was provided during a create of an API key. | [optional]
**iam_id** | **String** | The iam_id that this API key authenticates. | 
**account_id** | **String** | ID of the account that this API key authenticates for. | 
**apikey** | **String** | The API key value. This property only contains the API key value for the following cases: create an API key, update a service ID API key that stores the API key value as retrievable, or get a service ID API key that stores the API key value as retrievable. All other operations don't return the API key value, for example all user API key related operations, except for create, don't contain the API key value. | 
**history** | Option<[**Vec<models::EnityHistoryRecord>**](EnityHistoryRecord.md)> | History of the API key. | [optional]
**activity** | Option<[**models::Activity**](Activity.md)> |  | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


