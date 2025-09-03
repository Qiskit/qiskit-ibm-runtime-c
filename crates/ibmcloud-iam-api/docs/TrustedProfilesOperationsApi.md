# \TrustedProfilesOperationsApi

All URIs are relative to *https://iam.cloud.ibm.com*

Method | HTTP request | Description
------------- | ------------- | -------------
[**create_claim_rule**](TrustedProfilesOperationsApi.md#create_claim_rule) | **POST** /v1/profiles/{profile_id}/rules | Create claim rule for a trusted profile
[**create_link**](TrustedProfilesOperationsApi.md#create_link) | **POST** /v1/profiles/{profile_id}/links | Create link to a trusted profile
[**create_profile**](TrustedProfilesOperationsApi.md#create_profile) | **POST** /v1/profiles | Create a trusted profile
[**delete_claim_rule**](TrustedProfilesOperationsApi.md#delete_claim_rule) | **DELETE** /v1/profiles/{profile_id}/rules/{rule_id} | Delete a claim rule
[**delete_link**](TrustedProfilesOperationsApi.md#delete_link) | **DELETE** /v1/profiles/{profile_id}/links/{link_id} | Delete link to a trusted profile
[**delete_profile**](TrustedProfilesOperationsApi.md#delete_profile) | **DELETE** /v1/profiles/{profile_id} | Delete a trusted profile
[**delete_profile_identity**](TrustedProfilesOperationsApi.md#delete_profile_identity) | **DELETE** /v1/profiles/{profile_id}/identities/{identity_type}/{identifier_id} | Delete the identity that can assume the trusted profile. This API will update the trusted profile itself, thus calling it repeatedly for the same profile can lead to conflicts responded with HTTP code 409. Make sure to call this API only once in a few seconds for the same trusted profile.
[**get_claim_rule**](TrustedProfilesOperationsApi.md#get_claim_rule) | **GET** /v1/profiles/{profile_id}/rules/{rule_id} | Get a claim rule for a trusted profile
[**get_link**](TrustedProfilesOperationsApi.md#get_link) | **GET** /v1/profiles/{profile_id}/links/{link_id} | Get link to a trusted profile
[**get_profile**](TrustedProfilesOperationsApi.md#get_profile) | **GET** /v1/profiles/{profile_id} | Get a trusted profile
[**get_profile_identities**](TrustedProfilesOperationsApi.md#get_profile_identities) | **GET** /v1/profiles/{profile_id}/identities | Get a list of identities that can assume the trusted profile
[**get_profile_identity**](TrustedProfilesOperationsApi.md#get_profile_identity) | **GET** /v1/profiles/{profile_id}/identities/{identity_type}/{identifier_id} | Get the identity that can assume the trusted profile
[**list_claim_rules**](TrustedProfilesOperationsApi.md#list_claim_rules) | **GET** /v1/profiles/{profile_id}/rules | List claim rules for a trusted profile
[**list_links**](TrustedProfilesOperationsApi.md#list_links) | **GET** /v1/profiles/{profile_id}/links | List links to a trusted profile
[**list_profiles**](TrustedProfilesOperationsApi.md#list_profiles) | **GET** /v1/profiles | List trusted profiles
[**set_profile_identities**](TrustedProfilesOperationsApi.md#set_profile_identities) | **PUT** /v1/profiles/{profile_id}/identities | Update the list of identities that can assume the trusted profile
[**set_profile_identity**](TrustedProfilesOperationsApi.md#set_profile_identity) | **POST** /v1/profiles/{profile_id}/identities/{identity_type} | Add a specific identity that can assume the trusted profile
[**update_claim_rule**](TrustedProfilesOperationsApi.md#update_claim_rule) | **PUT** /v1/profiles/{profile_id}/rules/{rule_id} | Update claim rule for a trusted profile
[**update_profile**](TrustedProfilesOperationsApi.md#update_profile) | **PUT** /v1/profiles/{profile_id} | Update a trusted profile



## create_claim_rule

> models::ProfileClaimRule create_claim_rule(authorization, profile_id, profile_claim_rule_request)
Create claim rule for a trusted profile

Create a claim rule for a trusted profile. There is a limit of 20 rules per trusted profile.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**profile_id** | **String** | ID of the trusted profile to create a claim rule. | [required] |
**profile_claim_rule_request** | [**ProfileClaimRuleRequest**](ProfileClaimRuleRequest.md) | Request to create a claim rule for trusted profile. | [required] |

### Return type

[**models::ProfileClaimRule**](ProfileClaimRule.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## create_link

> models::ProfileLink create_link(profile_id, authorization, create_profile_link_request)
Create link to a trusted profile

Create a direct link between a specific compute resource and a trusted profile, rather than creating conditions that a compute resource must fulfill to apply a trusted profile.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**profile_id** | **String** | ID of the trusted profile. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**create_profile_link_request** | [**CreateProfileLinkRequest**](CreateProfileLinkRequest.md) | Request to create a Link to Trusted profile. | [required] |

### Return type

[**models::ProfileLink**](ProfileLink.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## create_profile

> models::TrustedProfile create_profile(authorization, create_trusted_profile_request)
Create a trusted profile

Create a trusted profile for a given account ID.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**create_trusted_profile_request** | [**CreateTrustedProfileRequest**](CreateTrustedProfileRequest.md) | Request to create a trusted profile. | [required] |

### Return type

[**models::TrustedProfile**](TrustedProfile.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## delete_claim_rule

> delete_claim_rule(profile_id, rule_id, authorization)
Delete a claim rule

Delete a claim rule. When you delete a claim rule, federated user or compute resources are no longer required to meet the conditions of the claim rule in order to apply the trusted profile.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**profile_id** | **String** | ID of the trusted profile. | [required] |
**rule_id** | **String** | ID of the claim rule to delete. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

 (empty response body)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: Not defined

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## delete_link

> delete_link(profile_id, link_id, authorization)
Delete link to a trusted profile

Delete a link between a compute resource and a trusted profile.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**profile_id** | **String** | ID of the trusted profile | [required] |
**link_id** | **String** | ID of the link | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

 (empty response body)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: Not defined

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## delete_profile

> delete_profile(profile_id, authorization)
Delete a trusted profile

Delete a trusted profile. When you delete trusted profile, compute resources and federated users are unlinked from the profile and can no longer apply the trusted profile identity.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**profile_id** | **String** | ID of the trusted profile. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

 (empty response body)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: Not defined

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## delete_profile_identity

> delete_profile_identity(profile_id, identity_type, identifier_id, authorization)
Delete the identity that can assume the trusted profile. This API will update the trusted profile itself, thus calling it repeatedly for the same profile can lead to conflicts responded with HTTP code 409. Make sure to call this API only once in a few seconds for the same trusted profile.

Delete the identity that can assume the trusted profile

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**profile_id** | **String** | ID of the trusted profile | [required] |
**identity_type** | **String** | Type of the identity | [required] |
**identifier_id** | **String** | Identifier of the identity that can assume the trusted profiles. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

 (empty response body)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: Not defined

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_claim_rule

> models::ProfileClaimRule get_claim_rule(profile_id, rule_id, authorization)
Get a claim rule for a trusted profile

A specific claim rule can be fetched for a given trusted profile ID and rule ID.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**profile_id** | **String** | ID of the trusted profile. | [required] |
**rule_id** | **String** | ID of the claim rule to get. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

[**models::ProfileClaimRule**](ProfileClaimRule.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_link

> models::ProfileLink get_link(profile_id, link_id, authorization)
Get link to a trusted profile

Get a specific link to a trusted profile by `link_id`.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**profile_id** | **String** | ID of the trusted profile | [required] |
**link_id** | **String** | ID of the link | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

[**models::ProfileLink**](ProfileLink.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_profile

> models::TrustedProfile get_profile(profile_id, authorization, include_activity)
Get a trusted profile

Retrieve a trusted profile by its `profile-id`. Only the trusted profile's data is returned (`name`, `description`, `iam_id`, etc.), not the federated users or compute resources that qualify to apply the trusted profile.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**profile_id** | **String** | ID of the trusted profile to get. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**include_activity** | Option<**bool**> | Defines if the entity's activity is included in the response. Retrieving activity data is an expensive operation, so only request this when needed. |  |[default to false]

### Return type

[**models::TrustedProfile**](TrustedProfile.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_profile_identities

> models::ProfileIdentitiesResponse get_profile_identities(profile_id, authorization)
Get a list of identities that can assume the trusted profile

Get a list of identities that can assume the trusted profile

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**profile_id** | **String** | ID of the trusted profile. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

[**models::ProfileIdentitiesResponse**](ProfileIdentitiesResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## get_profile_identity

> models::ProfileIdentityResponse get_profile_identity(profile_id, identity_type, identifier_id, authorization)
Get the identity that can assume the trusted profile

Get the identity that can assume the trusted profile

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**profile_id** | **String** | ID of the trusted profile | [required] |
**identity_type** | **String** | Type of the identity | [required] |
**identifier_id** | **String** | Identifier of the identity that can assume the trusted profiles. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

[**models::ProfileIdentityResponse**](ProfileIdentityResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## list_claim_rules

> models::ProfileClaimRuleList list_claim_rules(profile_id, authorization)
List claim rules for a trusted profile

Get a list of all claim rules for a trusted profile. The `profile-id` query parameter determines the profile from which to retrieve the list of claim rules.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**profile_id** | **String** | ID of the trusted profile. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

[**models::ProfileClaimRuleList**](ProfileClaimRuleList.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## list_links

> models::ProfileLinkList list_links(profile_id, authorization)
List links to a trusted profile

Get a list of links to a trusted profile.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**profile_id** | **String** | ID of the trusted profile | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |

### Return type

[**models::ProfileLinkList**](ProfileLinkList.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## list_profiles

> models::TrustedProfilesList list_profiles(account_id, authorization, name, pagesize, sort, order, include_history, pagetoken, filter)
List trusted profiles

List the trusted profiles in an account. The `account_id` query parameter determines the account from which to retrieve the list of trusted profiles.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**account_id** | **String** | Account ID to query for trusted profiles. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**name** | Option<**String**> | Name of the trusted profile to query. |  |
**pagesize** | Option<**i32**> | Optional size of a single page. Default is 20 items per page. Valid range is 1 to 100. |  |
**sort** | Option<**String**> | Optional sort property, valid values are name, description, created_at and modified_at. If specified, the items are sorted by the value of this property. |  |
**order** | Option<**String**> | Optional sort order, valid values are asc and desc. Default: asc. |  |[default to asc]
**include_history** | Option<**bool**> | Defines if the entity history is included in the response. |  |[default to false]
**pagetoken** | Option<**String**> | Optional Prev or Next page token returned from a previous query execution. Default is start with first page. |  |
**filter** | Option<**String**> | An optional filter query parameter used to refine the results of the search operation. For more information see [Filtering list results](#filter-list-results) section. |  |

### Return type

[**models::TrustedProfilesList**](TrustedProfilesList.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## set_profile_identities

> models::ProfileIdentitiesResponse set_profile_identities(profile_id, if_match, authorization, profile_identities_update_request)
Update the list of identities that can assume the trusted profile

Update the list of identities that can assume the trusted profile

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**profile_id** | **String** | ID of the trusted profile. | [required] |
**if_match** | **String** | Entity tag of the Identities to be updated. Specify the tag that you retrieved when reading the Profile Identities. This value helps identify parallel usage of this API. Pass * to indicate updating any available version, which may result in stale updates. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**profile_identities_update_request** | [**ProfileIdentitiesUpdateRequest**](ProfileIdentitiesUpdateRequest.md) | Request to update identities. | [required] |

### Return type

[**models::ProfileIdentitiesResponse**](ProfileIdentitiesResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## set_profile_identity

> models::ProfileIdentityResponse set_profile_identity(profile_id, identity_type, authorization, profile_identity_request)
Add a specific identity that can assume the trusted profile

Add a specific identity that can assume the trusted profile. This API will update the trusted profile itself, thus calling it repeatedly for the same profile can lead to conflicts responded with HTTP code 409. Make sure to call this API only once in a few seconds for the same trusted profile.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**profile_id** | **String** | ID of the trusted profile | [required] |
**identity_type** | **String** | Type of the identity | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**profile_identity_request** | [**ProfileIdentityRequest**](ProfileIdentityRequest.md) | Request to update identities. | [required] |

### Return type

[**models::ProfileIdentityResponse**](ProfileIdentityResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## update_claim_rule

> models::ProfileClaimRule update_claim_rule(profile_id, rule_id, authorization, if_match, profile_claim_rule_request)
Update claim rule for a trusted profile

Update a specific claim rule for a given trusted profile ID and rule ID.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**profile_id** | **String** | ID of the trusted profile. | [required] |
**rule_id** | **String** | ID of the claim rule to update. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**if_match** | **String** | Version of the claim rule to be updated. Specify the version that you retrived when reading list of claim rules. This value helps to identify any parallel usage of claim rule. Pass * to indicate to update any version available. This might result in stale updates.  | [required] |
**profile_claim_rule_request** | [**ProfileClaimRuleRequest**](ProfileClaimRuleRequest.md) | Request to update a claim rule. | [required] |

### Return type

[**models::ProfileClaimRule**](ProfileClaimRule.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)


## update_profile

> models::TrustedProfile update_profile(profile_id, authorization, if_match, update_trusted_profile_request)
Update a trusted profile

Update the name or description of an existing trusted profile.

### Parameters


Name | Type | Description  | Required | Notes
------------- | ------------- | ------------- | ------------- | -------------
**profile_id** | **String** | ID of the trusted profile to be updated. | [required] |
**authorization** | **String** | Authorization Token used for the request. The supported token type is a Cloud IAM Access Token. If the token is omitted the request will fail with BXNIM0308E: 'No authorization header found'. Make sure that the provided token has the required authority for the request. | [required] |
**if_match** | **String** | Version of the trusted profile to be updated. Specify the version that you retrived when reading list of trusted profiles. This value helps to identify any parallel usage of trusted profile. Pass * to indicate to update any version available. This might result in stale updates.  | [required] |
**update_trusted_profile_request** | [**UpdateTrustedProfileRequest**](UpdateTrustedProfileRequest.md) | Request to update a trusted profile. | [required] |

### Return type

[**models::TrustedProfile**](TrustedProfile.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

