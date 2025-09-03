# UpdateApiKeyRequest

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**name** | Option<**String**> | The name of the API key to update. If specified in the request the parameter must not be empty. The name is not checked for uniqueness. Failure to this will result in an Error condition. | [optional]
**description** | Option<**String**> | The description of the API key to update. If specified an empty description will clear the description of the API key. If a non empty value is provided the API key will be updated. | [optional]
**support_sessions** | Option<**bool**> | Defines whether you can manage CLI login sessions for the API key. When `true`, sessions are created and can be reviewed or revoked. When `false`, no sessions are tracked. To block access, delete or rotate the API key. Available only for user API keys. | [optional]
**action_when_leaked** | Option<**String**> | Defines the action to take when API key is leaked, valid values are 'none', 'disable' and 'delete'. | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


