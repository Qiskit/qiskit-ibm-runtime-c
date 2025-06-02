# CreateServiceIdRequest

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**account_id** | **String** | ID of the account the service ID belongs to. | 
**name** | **String** | Name of the Service Id. The name is not checked for uniqueness. Therefore multiple names with the same value can exist. Access is done via the UUID of the Service Id. | 
**description** | Option<**String**> | The optional description of the Service Id. The 'description' property is only available if a description was provided during a create of a Service Id. | [optional]
**unique_instance_crns** | Option<**Vec<String>**> | Optional list of CRNs (string array) which point to the services connected to the service ID. | [optional]
**apikey** | Option<[**models::ApiKeyInsideCreateServiceIdRequest**](ApiKeyInsideCreateServiceIdRequest.md)> |  | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


