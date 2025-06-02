# CreateJobRequestOneOf

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**program_id** | **String** | ID of the program to be executed | 
**backend** | **String** | Name that identifies the backend on which to run the program. | 
**runtime** | Option<**String**> | Name and tag of the image to use when running a program (IBM Quantum channel users only). Should follow the pattern \"name:tag\". | [optional]
**tags** | Option<**Vec<String>**> | List of job or program tags | [optional]
**log_level** | Option<**String**> | Logging level of the program | [optional]
**cost** | Option<**i32**> | Cost of the job as the estimated time it should take to complete (in seconds). Should not exceed the cost of the program | [optional]
**session_id** | Option<**String**> | Identifier of the session that the job is a part of | [optional]
**params** | Option<[**models::CreateJobRequestOneOfAllOfParams**](create_job_request_oneOf_allOf_params.md)> |  | [optional]
**private** | Option<**bool**> | When set to true, input parameters are not returned, and the results can only be read once. After the job is completed, input parameters are deleted from the service. After the results are read, they are deleted from the service. When set to false, the input parameters and results follow the standard retention behavior of the API. Only returned in the response if the value is true, otherwise it is omitted. | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


