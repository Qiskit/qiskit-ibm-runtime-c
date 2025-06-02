# ProfileClaimRuleRequest

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**context** | Option<[**models::ResponseContext**](ResponseContext.md)> |  | [optional]
**name** | Option<**String**> | Name of the claim rule to be created or updated | [optional]
**r#type** | **String** | Type of the claim rule, either 'Profile-SAML' or 'Profile-CR' | 
**realm_name** | Option<**String**> | The realm name of the Idp this claim rule applies to. This field is required only if the type is specified as 'Profile-SAML'. | [optional]
**cr_type** | Option<**String**> | The compute resource type the rule applies to, required only if type is specified as 'Profile-CR'. Valid values are VSI, IKS_SA, ROKS_SA. | [optional]
**expiration** | Option<**i32**> | Session expiration in seconds, only required if type is 'Profile-SAML'. | [optional]
**conditions** | [**Vec<models::ProfileClaimRuleConditions>**](ProfileClaimRuleConditions.md) | Conditions of this claim rule. | 

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


