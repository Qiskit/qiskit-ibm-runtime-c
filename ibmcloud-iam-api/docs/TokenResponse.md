# TokenResponse

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**access_token** | Option<**String**> | The IAM access token that can be used to invoke various IBM Cloud APIs. Use this token with the prefix Bearer in the HTTP header Authorization for invocations of IAM compatible APIs. | [optional]
**refresh_token** | Option<**String**> | (optional) A refresh token that can be used to get a new IAM access token if that token is expired. When using the default client (no basic authorization header) as described in this documentation, this refresh_token cannot be used to retrieve a new IAM access token. When the IAM access token is about to be expired, use the API key to create a new access token. | [optional]
**delegated_refresh_token** | Option<**String**> | (optional) A delegated refresh token that can only be consumed by the clients that have been specified in the API call as 'receiver_client_ids' | [optional]
**token_type** | Option<**String**> | The type of the token. Currently, only Bearer is returned. | [optional]
**expires_in** | Option<**i32**> | Number of seconds until the IAM access token will expire. | [optional]
**expiration** | Option<**i32**> | Number of seconds counted since January 1st, 1970, until the IAM access token will expire. | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


