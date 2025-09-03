# CatalogCategoryService

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**id** | **String** | The service identifier in Globacatalog | 
**display_name** | **String** | The service display name in the selected language | 
**name** | **String** | The name of the service. It can be used to query GhoST property service_name | 
**kind** | **String** | The kind of the Global catalog entry. Example values are `service`, `runtime`, `application`, `buildpack` | 
**gst_support** | **bool** | A flag that indicates whether the service is onboarded in GhoST | 
**gst_data** | Option<[**Vec<models::GstData>**](GstData.md)> | gst_data | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


