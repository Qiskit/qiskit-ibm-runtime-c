# ProfileClaimRule

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**id** | **String** | the unique identifier of the claim rule | 
**entity_tag** | **String** | version of the claim rule | 
**created_at** | **String** | If set contains a date time string of the creation date in ISO format. | 
**modified_at** | Option<**String**> | If set contains a date time string of the last modification date in ISO format. | [optional]
**name** | Option<**String**> | The optional claim rule name | [optional]
**r#type** | **String** | Type of the claim rule, either 'Profile-SAML' or 'Profile-CR' | 
**realm_name** | Option<**String**> | The realm name of the Idp this claim rule applies to | [optional]
**expiration** | **i32** | Session expiration in seconds | 
**cr_type** | Option<**String**> | The compute resource type. Not required if type is Profile-SAML. Valid values are VSI, IKS_SA, ROKS_SA. | [optional]
**conditions** | [**Vec<models::ProfileClaimRuleConditions>**](ProfileClaimRuleConditions.md) | Conditions of this claim rule. | 

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


