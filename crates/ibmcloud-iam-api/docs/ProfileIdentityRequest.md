# ProfileIdentityRequest

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**identifier** | **String** | Identifier of the identity that can assume the trusted profiles. This can be a user identifier (IAM id), serviceid or crn. Internally it uses account id of the service id for the identifier 'serviceid' and for the identifier 'crn' it uses account id contained in the CRN. | 
**r#type** | **String** | Type of the identity | 
**accounts** | Option<**Vec<String>**> | Only valid for the type user. Accounts from which a user can assume the trusted profile. | [optional]
**description** | Option<**String**> | Description of the identity that can assume the trusted profile. This is optional field for all the types of identities. When this field is not set for the identity type 'serviceid' then the description of the service id is used. Description is recommended for the identity type 'crn' E.g. 'Instance 1234 of IBM Cloud Service project' | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


