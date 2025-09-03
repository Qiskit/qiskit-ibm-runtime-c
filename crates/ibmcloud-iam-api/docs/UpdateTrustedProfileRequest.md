# UpdateTrustedProfileRequest

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**name** | Option<**String**> | The name of the trusted profile to update. If specified in the request the parameter must not be empty. The name is checked for uniqueness. Failure to this will result in an Error condition. | [optional]
**description** | Option<**String**> | The description of the trusted profile to update. If specified an empty description will clear the description of the trusted profile. If a non empty value is provided the trusted profile will be updated. | [optional]
**email** | Option<**String**> | The email of the profile to update. If specified an empty email will clear the email of the profile. If an non empty value is provided the trusted profile will be updated. | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


