# IdBasedMfaEnrollment

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**trait_account_default** | **String** | Defines the MFA trait for the account. Valid values:   * NONE - No MFA trait set   * NONE_NO_ROPC- No MFA, disable CLI logins with only a password   * TOTP - For all non-federated IBMId users   * TOTP4ALL - For all users   * LEVEL1 - Email-based MFA for all users   * LEVEL2 - TOTP-based MFA for all users   * LEVEL3 - U2F MFA for all users  | 
**trait_user_specific** | Option<**String**> | Defines the MFA trait for the account. Valid values:   * NONE - No MFA trait set   * NONE_NO_ROPC- No MFA, disable CLI logins with only a password   * TOTP - For all non-federated IBMId users   * TOTP4ALL - For all users   * LEVEL1 - Email-based MFA for all users   * LEVEL2 - TOTP-based MFA for all users   * LEVEL3 - U2F MFA for all users  | [optional]
**trait_effective** | **String** | Defines the MFA trait for the account. Valid values:   * NONE - No MFA trait set   * NONE_NO_ROPC- No MFA, disable CLI logins with only a password   * TOTP - For all non-federated IBMId users   * TOTP4ALL - For all users   * LEVEL1 - Email-based MFA for all users   * LEVEL2 - TOTP-based MFA for all users   * LEVEL3 - U2F MFA for all users  | 
**complies** | **bool** | The enrollment complies to the effective requirement. | 
**comply_state** | Option<**String**> | Defines comply state for the account. Valid values:   * NO - User does not comply in the given account.   * ACCOUNT- User complies in the given account, but does not comply in at least one of the other account memberships.   * CROSS_ACCOUNT - User complies in the given account and across all other account memberships.  | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


