# IdentityPreferenceResponse

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**service** | Option<**String**> | Service of the preference | [optional]
**id** | Option<**String**> | Unique ID of the preference | [optional]
**account_id** | Option<**String**> | Account ID of the preference, only present for scope 'account' | [optional]
**scope** | Option<**String**> | Scope of the preference, 'global' or 'account' | [optional]
**value_string** | Option<**String**> | String value of the preference, only one value property is set, either 'value_string' or 'value_list_of_strings' is present. | [optional]
**value_list_of_strings** | Option<**Vec<String>**> | List of value of the preference, only one value property is set, either 'value_string' or 'value_list_of_strings' is present. | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


