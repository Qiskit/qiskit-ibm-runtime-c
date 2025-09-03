# TermsAggregationResultTerms

## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**doc_count_error_upper_bound** | Option<**i32**> | An upper bound of the error on the document counts for each term. | [optional]
**sum_other_doc_count** | Option<**i32**> | When there are lots of unique terms, the buckets array only returns the top terms you requested with the `size` request attribute. The `sum_other_doc_count` is the sum of the document counts for all the buckets that are not part of the response. | [optional]
**next_query** | Option<**String**> | The Lucene-formatted query string to use to get the next batch of unique term values. It is set only if you set in the request body the order to `term:asc`, and if there are more than `size` requested unique terms to return, which is signaled by a sum_other_doc_count greater than zero. | [optional]
**buckets** | [**Vec<models::TermsAggregationResultTermsBucketsInner>**](TermsAggregationResult_terms_buckets_inner.md) | The buckets of field terms | 

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)


