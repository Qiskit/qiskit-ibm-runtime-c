# AccountSettingsAssignedTemplatesSection

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**template_id** | Option<**String**> | Template Id | [optional]
**template_version** | Option<**i64**> | Template version | [optional]
**template_name** | Option<**String**> | Template name | [optional]
**restrict_create_service_id** | Option<**String**> | Defines whether or not creating a service ID is access controlled. Valid values:   * RESTRICTED - only users assigned the 'Service ID creator' role on the IAM Identity Service can create service IDs, including the account owner   * NOT_RESTRICTED - all members of an account can create service IDs   * NOT_SET - to 'unset' a previous set value | [optional][default to NotSet]
**restrict_create_platform_apikey** | Option<**String**> | Defines whether or not creating platform API keys is access controlled. Valid values:   * RESTRICTED - to apply access control   * NOT_RESTRICTED - to remove access control   * NOT_SET - to 'unset' a previous set value | [optional][default to NotSet]
**allowed_ip_addresses** | Option<**String**> | Defines the IP addresses and subnets from which IAM tokens can be created for the account. | [optional]
**mfa** | Option<**String**> | Defines the MFA requirement for the user. Valid values:   * NONE - No MFA trait set   * NONE_NO_ROPC- No MFA, disable CLI logins with only a password   * TOTP - For all non-federated IBMId users   * TOTP4ALL - For all users   * LEVEL1 - Email-based MFA for all users   * LEVEL2 - TOTP-based MFA for all users   * LEVEL3 - U2F MFA for all users | [optional]
**user_mfa** | Option<[**Vec<models::EffectiveAccountSettingsUserMfa>**](EffectiveAccountSettingsUserMFA.md)> | List of users that are exempted from the MFA requirement of the account. | [optional]
**session_expiration_in_seconds** | Option<**String**> | Defines the session expiration in seconds for the account. Valid values:   * Any whole number between between '900' and '86400'   * NOT_SET - To unset account setting and use service default | [optional][default to 86400]
**session_invalidation_in_seconds** | Option<**String**> | Defines the period of time in seconds in which a session will be invalidated due to inactivity. Valid values:   * Any whole number between '900' and '7200'   * NOT_SET - To unset account setting and use service default | [optional][default to 7200]
**max_sessions_per_identity** | Option<**String**> | Defines the max allowed sessions per identity required by the account. Valid values:   * Any whole number greater than 0   * NOT_SET - To unset account setting and use service default | [optional]
**system_access_token_expiration_in_seconds** | Option<**String**> | Defines the access token expiration in seconds. Valid values:   * Any whole number between '900' and '3600'   * NOT_SET - To unset account setting and use service default | [optional][default to 3600]
**system_refresh_token_expiration_in_seconds** | Option<**String**> | Defines the refresh token expiration in seconds. Valid values:   * Any whole number between '900' and '259200'   * NOT_SET - To unset account setting and use service default | [optional][default to 259200]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


