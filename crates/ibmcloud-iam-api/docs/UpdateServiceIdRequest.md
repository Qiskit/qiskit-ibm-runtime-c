# UpdateServiceIdRequest

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**name** | Option<**String**> | The name of the service ID to update. If specified in the request the parameter must not be empty. The name is not checked for uniqueness. Failure to this will result in an Error condition. | [optional]
**description** | Option<**String**> | The description of the service ID to update. If specified an empty description will clear the description of the service ID. If an non empty value is provided the service ID will be updated. | [optional]
**unique_instance_crns** | Option<**Vec<String>**> | List of CRNs which point to the services connected to this service ID. If specified an empty list will clear all existing unique instance crns of the service ID. | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


