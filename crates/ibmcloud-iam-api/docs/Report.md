# Report

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**created_by** | **String** | IAMid of the user who triggered the report | 
**reference** | **String** | Unique reference used to generate the report | 
**report_duration** | **String** | Duration in hours for which the report is generated. | 
**report_start_time** | **String** | Start time of the report | 
**report_end_time** | **String** | End time of the report | 
**users** | Option<[**Vec<models::UserActivity>**](UserActivity.md)> | List of users | [optional]
**apikeys** | Option<[**Vec<models::ApikeyActivity>**](ApikeyActivity.md)> | List of apikeys | [optional]
**serviceids** | Option<[**Vec<models::EntityActivity>**](EntityActivity.md)> | List of serviceids | [optional]
**profiles** | Option<[**Vec<models::EntityActivity>**](EntityActivity.md)> | List of profiles | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


