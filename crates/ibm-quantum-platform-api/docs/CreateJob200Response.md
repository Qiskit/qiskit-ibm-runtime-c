# CreateJob200Response

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**id** | **String** | Job ID | 
**backend** | **String** | Backend selected to run job | 
**session_id** | Option<**String**> | Id of the session associated with the job | [optional]
**private** | Option<**bool**> | When set to true, input parameters are not returned, and the results can only be read once. After the job is completed, input parameters are deleted from the service. After the results are read, they are deleted from the service. When set to false, the input parameters and results follow the standard retention behavior of the API. Only returned in the response if the value is true, otherwise it is omitted. | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


