# OidcExceptionResponse

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**context** | Option<[**models::ExceptionResponseContext**](ExceptionResponseContext.md)> |  | [optional]
**error_code** | **String** | Error message code of the REST Exception.  | 
**error_message** | **String** | Error message of the REST Exception. Error messages are derived base on the input locale of the REST request and the available Message catalogs. Dynamic fallback to 'us-english' is happening if no message catalog is available for the provided input locale. | 
**error_details** | Option<**String**> | Error details of the REST Exception.  | [optional]
**requirements** | Option<[**models::MfaRequirementsResponse**](MFARequirementsResponse.md)> |  | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


