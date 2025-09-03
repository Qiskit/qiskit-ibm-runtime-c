# TermsAggregationTerms

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**field** | **String** | The field on which to perform the aggregation. | 
**size** | Option<**i32**> | The number of buckets to return, one per unique term. If you have more than 1000 different terms in the aggregating field, and you want to get them, then you must perform multiple aggregations leveraging the query parameter to aggregate on disjointed results set. For example, aggregating on the account_id field may produce a high number of buckets, depending on the query, which may exceed the limit of 1000. | [optional][default to 10]
**order** | Option<**String**> | By default, the terms aggregation orders terms by descending document count. You can also order by term in ascending and descending orders. You cannot order by document count in ascending order. | [optional][default to CountDesc]
**missing** | Option<**String**> | Defines how documents that are missing a value should be treated. By default they will be ignored but it is also possible to treat them as if they had a value. | [optional]

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


