// This code is part of Qiskit.
//
// (C) Copyright IBM 2025
//
// This code is licensed under the Apache License, Version 2.0. You may
// obtain a copy of this license in the LICENSE.txt file in the root directory
// of this source tree or at http://www.apache.org/licenses/LICENSE-2.0.
//
// Any modifications or derivative works of this code must retain this
// copyright notice, and modified files need to carry a notice indicating
// that they have been altered from the originals.

#include <qiskit.h>

typedef struct Service Service;
typedef struct Job Job;
typedef struct Backend Backend;
typedef struct BackendSearchResults BackendSearchResults;
typedef struct Samples Samples;
typedef struct Counts Counts;
typedef struct ExpectationValues ExpectationValues;

/**
 * Allocate a new Qiskit IBM Runtime Client service instance.
 *
 * You must free the service with ``qkrt_service_free`` when you're done
 * with it.
 *
 * @param[out] out A pointer to where the newly allocated service's handle
 *     will be written.
 *
 * @return An exit code to indicate the status of the call.
 *
 * # Example
 *
 *     Service *service;
 *     int res = qkrt_service_new(&service);
 *     if (res != 0) {
 *         printf("service new failed with code: %d\n", res);
 *         return res;
 *     }
 *
 *     // do something with the service...
 *
 *     qkrt_service_free(service);
 */
extern int32_t qkrt_service_new(Service **out);

/**
 * The configuration input to allocate a new service with custom options.
 *
 * All fields can be null pointers and if so the default value will be used.
 * You can create a new ``QkrtServiceConfig`` instance with all null values using
 * ``qkrt_default_service_config``.
 */
typedef struct {
    /// The token to use to authenticate against IBM cloud with. By default it will be read from the
    /// specified config file
    const char *token;
    /// The url to use for connecting to IBM Cloud Identity and Access Management. If this is a null pointer by
    /// default it will be "https://iam.cloud.ibm.com".
    const char *iam_url;
    /// The url to use for connecting to the IBM Quantum Platform. If this is a null pointer by
    /// default it will be "https://quantum.cloud.ibm.com".
    const char *iqp_url;
    /// The url to use for connecting to the IBM Cloud Global Search. If this is a null pointer by
    /// default it will be "https://api.global-search-tagging.cloud.ibm.com".
    const char *global_search_url;
    /// The user agent to set for all requests made from the service. By default it will be: "qiskit-ibm-runtime-rs/0.0.1"
    const char *user_agent;
    /// The filename to the json configuration to read from. By default it will use from "$HOME/.qiskit/qiskit-ibm.json".
    const char *filename;
    /// The account name to use inside the specified json file. By default it will use either "default",
    /// "default-ibm-quantum-platform", "default-ibm-cloud" in that order.
    const char *account_name;
} QkrtServiceConfig;

/**
 * Create a new ``QkrtServiceConfig`` object with all null pointer values. This indicates the default
 * should be used for all fields. It is typically used to quickly create a struct and set a few custom
 * values. For example:
 *
 * ```c
 * QkrtServiceConfig config = qkrt_default_service_config();
 * config.user_agent = "CustomClient_0.2";
 * ```
 */
extern QkrtServiceConfig qkrt_default_service_config();


/**
 * Allocate a new Qiskit IBM Runtime Client service instance using a custom configuration
 *
 * You must free the service with ``qkrt_service_free`` when you're done with it.
 *
 * @param config A pointer to the configuration struct to use for allocating the new service instance.
 * @param[out] out A pointer to where the newly allocate service's handle will be written
 *
 * @return An exit code to indicate the status of the call.
 *
 * # Example                                                                                                
 *                                                                                                            
 *     QkrtServiceConfig config = qkrt_default_service_config();                                            
 *     config.user_agent = "CustomClient_0.2";                                                                       
 *                                                                                                            
 *     Service *service;
 *     int res = qkrt_service_new_from_config(&config, &service);
 *     if (res != 0) {
 *         printf("service new failed with code: %d\n", res);
 *         return res;
 *     }
 *
 *     // do something with the service...
 *
 *     qkrt_service_free(service);
 */
extern int32_t qkrt_service_new_from_config(const QkrtServiceConfig *config, Service **out);

/**
 * Free a Qiskit IBM Runtime Client service instance.
 *
 * @param service A handle to the service to free.
 */
extern void qkrt_service_free(Service *service);

/**
 * Search the backends available via the provided service handle.
 *
 * If the service was configured without specifying an instance, the search
 * results will include all backends accessible via the account.
 *
 * @param[out] out A pointer to where the newly allocated search result listing's
 *     handle will be written.
 * @param service A handle to the service to search.
 *
 * @return An exit code to indicate the status of the call.
 */
extern int32_t qkrt_backend_search(BackendSearchResults **out, Service *service);

extern void qkrt_backend_search_results_free(BackendSearchResults *results);

extern uint64_t qkrt_backend_search_results_length(BackendSearchResults *results);

extern Backend** qkrt_backend_search_results_data(BackendSearchResults *results);

/**
 * A helper function to find and return a handle to the backend that is the least busy
 * (fewest queued jobs) from the given search results.
 *
 * The function does not perform additional network requests.
 *
 * @param results The results to search.
 *
 * @return A handle to the least busy backend present within the provided search results.
 *     If the results list is empty, NULL.
 */
extern Backend* qkrt_backend_search_results_least_busy(BackendSearchResults *results);

extern QkTarget* qkrt_get_backend_target(Service *service , Backend *backend);

/**
 * Get the name of the provided backend.
 *
 * @param A handle to the backend.
 *
 * @return The name of the backend.
 */
extern const char* qkrt_backend_name(Backend *backend);

/**
 * Get the CRN of the instance associated with the provided backend.
 *
 * @param A handle to the backend.
 *
 * @return The instance CRN of the backend.
 */
extern const char* qkrt_backend_instance_crn(Backend *backend);

/**
 * Get the name of the instance associated with the provided backend.
 *
 * @param A handle to the backend.
 *
 * @return The instance name of the backend.
 */
extern const char* qkrt_backend_instance_name(Backend *backend);

/**
 * Submit a new job given a circuit and the backend to run it on.
 *
 * You must free the allocated job instance with ``qkrt_job_free`` when you're done
 * with it.
 *
 * @param[out] out A pointer to where the newly allocated job's handle will be written
 * @param service A handle to the service.
 * @param backend A handle to the backend.
 * @param circuit A handle to the circuit to run.
 * @param shots The number of shots for this run.
 * @param runtime The name of the runtime.
 *
 * @return An exit code to indicate the status of the call.
 */
extern int32_t qkrt_sampler_job_run(Job **out, Service *service, Backend *backend, QkCircuit *circuit, int32_t shots, char *runtime);

/**
 * Submit a new estimator job given a circuit, observable, and the backend to run it on.
 *
 * You must free the allocated job instance with ``qkrt_job_free`` when you're done
 * with it.
 *
 * @param[out] out A pointer to where the newly allocated job's handle will be written
 * @param service A handle to the service.
 * @param backend A handle to the backend.
 * @param circuit A handle to the circuit to run.
 * @param observable A handle to the observable to estimate
 * @param runtime The name of the runtime.
 *
 * @return An exit code to indicate the status of the call.
 */
extern int32_t qkrt_estimator_job_run(Job **out, Service *service, Backend *backend, QkCircuit *circuit, QkObs *observable, char *runtime);

/**
 * Check the status of the provided job.
 *
 * @param[out] out A pointer to where the resulting job status will be written.
 * @param service The service handle.
 * @param job The handle of the job to query.
 *
 * @return An exit code to indicate the status of the call.
 */
extern int32_t qkrt_job_status(uint32_t *out, Service *service, Job *job);

/**
 * Free the provided job.
 *
 * @param the handle of the job to free.
 */
extern void qkrt_job_free(Job *job);

extern void generate_qpy(QkCircuit *circuit, char *filename);

/**
 * Fetch the results of the provided sampler job.
 *
 * You must free the allocated samples with ``qkrt_samples_free`` when you are
 * done with them.
 *
 * @param[out] out A pointer to where the newly allocated samples' handle will be
 *     written.
 * @param service The service handle.
 * @param job The handle of the job to fetch the results of.
 *
 * @return An exit code to indicate the status of the call.
 */
extern int32_t qkrt_sampler_job_results(Samples **out, Service *service, Job *job);

/**
 * Fetch the results of the provided estimator job.
 *
 * You must free the allocated samples with ``qkrt_samples_free`` when you are
 * done with them.
 *
 * @param[out] out A pointer to where the newly allocated samples' handle will be
 *     written.
 * @param service The service handle.
 * @param job The handle of the job to fetch the results of.
 *
 * @return An exit code to indicate the status of the call.
 */
extern int32_t qkrt_estimator_job_results(ExpectationValues **out, Service *service, Job *job);

extern size_t qkrt_samples_num_samples(const Samples *samples);

extern size_t qkrt_expectation_values_num_evs(ExpectationValues *evs);

/**
 * Get a specific sample by index.
 *
 * @param samples The handle of the samples.
 * @param index The index of the sample to retrieve.
 */
extern char* qkrt_samples_get_sample(const Samples *samples, size_t index);

/**
 * Get a specified expectation value by index
 * @param evs The handle of the expetation values returned from `qkrt_estimator_job_results`.
 * @param index The index to get the expectation value of from `evs`.
 */
extern double qkrt_expectation_values_get_ev(ExpectationValues *evs, size_t index);

/**
 * Copy the expectation values into a double array.
 *
 * @param evs The handle to the expectation values from an estimator job
 * @param out A pointer to the double array that the expectation values will be copied
 *    into. The allocation this points to must have a sufficient space to store the
 *    length of `evs`. You can use `qkrt_expectation_values_num_evs` to check the
 *    length of `evs`.
 */
extern void qkrt_expectation_values_copy_into(ExpectationValues *evs, double *out);

/**
 * Free the provided samples.
 *
 * @param samples The handle of the samples to free.
 */
extern void qkrt_samples_free(Samples *samples);

/**
 * Create a counts histogram from a Samples array
 *
 * @param samples the samples to read and create a new Counts from
 *
 * @returns A pointer to the new Counts.
 */
extern Counts *qkrt_samples_to_counts(Samples *samples);

/**
 * Get the number of entries in a Counts
 */
extern size_t qkrt_counts_length(Counts *counts);

/**
 * Display the contents of the histogram to stdout
 */
extern void qkrt_counts_display(Counts *counts);

/**
 * Return the most frequent sample from the counts object
 *
 * This function will panic if the counts histogram is empty. You should
 * check that `qkrt_counts_length` is greater than 0 before calling
 * this function.
 */
extern char *qkrt_counts_most_frequent(Counts *counts);

/**
 * Return the least frequent sample from the counts object
 *
 * This function will panic if the counts histogram is empty. You should
 * check that `qkrt_counts_length` is greater than 0 before calling
 * this function.
 */
extern char *qkrt_counts_least_frequent(Counts *counts);

/**
 * Sort in-place the counts histogram in order of counts for each sample
 *
 * @param counts The counts object to sample
 * @param most_frequent_first If true reverse the sorting so the sample with the largest counts is first
 */
extern void qkrt_counts_sort_by_frequency(Counts *counts, bool most_frequent_first);

/**
 * Look up a sample's count
 *
 * @param counts The counts object to sample
 * @param sample The sample to look up the count for
 * @returns the count of a particular sample in the count histogram. If the
 *    sample is not present in histogram a value of 18446744073709551615 (the
 *    max value for the integer type uint64_t) will be returned to indicate
 *    it is not present.
 */
extern uint64_t qkrt_counts_get_by_sample(Counts *counts, char *sample);

/**
 * A count entry from a Counts.
 *
 * The `name` field contains a pointer to a separate string copy and this struct will need to be
 * cleared with `qkrt_count_clear` to free that allocation.
 */
typedef struct {
    /// The sample string.
    char *name;
    /// The number of occurrences of the sample string
    uint64_t count;
} QkrtCount;

/**
 * Clear the contents of a QkrtCount object
 *
 * This function will clear the contents of a QkrtCount object. It will free
 * the string `name` field and set the pointer to null and reset the `count`
 * value to 0. This is typically so you can reuse a single `QkrtCount` object with
 * multiple calls to `qkrt_counts_get_count`. For example, something like:
 *
 * ```c
 * void function(Counts *counts) {
 *   size_t length = qkrt_counts_length(counts);
 *   QkrtCount count = {null, 0};
 *   for (size_t i = 0; i < length; i++) {
 *       qkrt_counts_get_count(counts, i, &count);
 *       qkrt_count_clear(&count);
 *   }
 * }
 * ```
 *
 * @param count A pointer to the count object to clear. This must be a valid
 * aligned pointer. It can be null in which case this function is a no-op.
 *
 */
extern void qkrt_count_clear(QkrtCount *count);
/**
 * Get the count from the counts histogram by index
 *
 * @param counts A pointer to the counts histogram to read the count from
 * @param index The index to get the count for
 * @param out_count A pointer to the QkrtCount object to write the count data into.
 *     Note that you must call `qkrt_count_clear()` after processing
 *     the count to avoid a memory leak.
 *
 * @return An exit code to indicate whether the index was valid. It will be non-zero
 * if the index is not valid
 */
extern int32_t qkrt_counts_get_count(Counts *counts, size_t index, QkrtCount *out_count);

/**
 * Free the provided counts.
 *
 * @param counts The handle of the counts to free
 */
extern void qkrt_counts_free(Counts *counts);

/**
 * Free the provided array of expectation values.
 *
 * @param The handle of the expectation values to free.
 */
extern void qkrt_expectation_values_free(ExpectationValues *evs);

/**
 * Free the provided string.
 *
 * @param The string to free.
 */
extern void qkrt_str_free(char *string);
