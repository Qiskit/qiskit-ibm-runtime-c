# ApikeyActivity

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**id** | **String** | Unique id of the apikey. | 
**name** | Option<**String**> | Name provided during creation of the apikey. | [optional]
**r#type** | **String** | Type of the apikey. Supported values are `serviceid` and `user`. | 
**serviceid** | Option<[**models::ApikeyActivityServiceid**](ApikeyActivity_serviceid.md)> |  | [optional]
**user** | Option<[**models::ApikeyActivityUser**](ApikeyActivity_user.md)> |  | [optional]
**last_authn** | Option<**String**> | Time when the apikey was last authenticated. | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


