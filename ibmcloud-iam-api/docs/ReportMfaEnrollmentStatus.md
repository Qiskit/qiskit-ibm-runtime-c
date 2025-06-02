# ReportMfaEnrollmentStatus

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**created_by** | **String** | IAMid of the user who triggered the report | 
**reference** | **String** | Unique reference used to generate the report | 
**report_time** | **String** | Date time at which report is generated. Date is in ISO format. | 
**account_id** | **String** | BSS account id of the user who triggered the report | 
**ims_account_id** | Option<**String**> | IMS account id of the user who triggered the report | [optional]
**users** | Option<[**Vec<models::UserReportMfaEnrollmentStatus>**](UserReportMfaEnrollmentStatus.md)> | List of users | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


