# EffectiveAccountSettingsUserMfa

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**iam_id** | **String** | The iam_id of the user. | 
**mfa** | **String** | Defines the MFA requirement for the user. Valid values:   * NONE - No MFA trait set   * NONE_NO_ROPC- No MFA, disable CLI logins with only a password   * TOTP - For all non-federated IBMId users   * TOTP4ALL - For all users   * LEVEL1 - Email-based MFA for all users   * LEVEL2 - TOTP-based MFA for all users   * LEVEL3 - U2F MFA for all users | 
**name** | Option<**String**> | name of the user account. | [optional]
**user_name** | Option<**String**> | userName of the user. | [optional]
**email** | Option<**String**> | email of the user. | [optional]
**description** | Option<**String**> | optional description. | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


