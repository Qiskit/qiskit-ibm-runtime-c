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
typedef struct CountsHistogram CountsHistogram;

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
 * Fetch the results of the provided job.
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
extern int32_t qkrt_job_results(Samples **out, Service *service, Job *job);

extern size_t qkrt_samples_num_samples(const Samples *samples);

/**
 * Get a specific sample by index.
 *
 * @param samples The handle of the samples.
 * @param index The index of the sample to retrieve.
 */
extern char* qkrt_samples_get_sample(const Samples *samples, size_t index);

/**
 * Free the provided samples.
 *
 * @param samples The handle of the samples to free.
 */
extern void qkrt_samples_free(Samples *samples);

/**
 * Create a counts histogram from a Samples array
 *
 * @param samples the samples to read and create a new CountsHistogram from
 *
 * @returns A pointer to the new CountsHistogram.
 */
extern CountsHistogram *qkrt_samples_to_counts_histogram(Samples *samples);

/**
 * Get the number entries in a CountsHistogram
 */
extern size_t qkrt_counts_histogram_length(CountsHistogram *counts);

/**
 * Display the contents of the histogram to stdout
 */
extern void qkrt_counts_histogram_display(CountsHistogram *counts);

/**
 * Return the most frequent sample from the counts object
 */
extern char *qkrt_counts_histogram_most_frequent(CountsHistogram *counts);

/**
 * Return the most frequent sample from the counts object
 */
extern char *qkrt_counts_histogram_least_frequent(CountsHistogram *counts);

/**
 * Sort in-place the counts histogram in order of counts for each sample
 *
 * @param counts The counts object to sample
 * @param most_frequent_first If true reverse the sorting so the sample with the largest counts is first
 */
extern void qkrt_counts_histogram_sort_by_frequency(CountsHistogram *counts, bool most_frequent_first);

/**
 * A count entry from a CountsHistogram.
 */
typedef struct {
    // The sample string. This is a pointer to an owned copy and must be freed.
    char *name;
    // The number of occurences of the sample string
    uint64_t count;
} QkrtCount;
typedef struct {
    /// The sample string. This is a pointer to an owned copy and must be freed.
    char *name;
    /// The number of occurences of the sample string
    uint64_t count;
} QkrtCount;
/**
 * Get the count from the counts histogram by index
 *
 * @param counts A pointer to the counts histogram to read the count from
 * @param index The index to get the count for
 * @param out_count A pointer to the QkrtCount object to write the count data into.
 *     Note that this does not free the name field you should call `qkrt_str_free()` on
 *     the name field if a string has been set in the name already. Otherwise passing an
 *     existing QkrtCount in for this will cause a memory leak.
 *
 * @return An exit code to indicate whether the index was valid. It will be non-zero
 * if the index is not valid
 */
extern int32_t qkrt_counts_histogram_get_count(CountsHistogram *counts, size_t index, QkrtCount *out_count);

/**
 * Free the provided counts.
 *
 * @param counts The handle of the counts to free
 */
extern void qkrt_counts_histogram_free(CountsHistogram *counts);

/**
 * Free the provided string.
 *
 * @param The string to free.
 */
extern void qkrt_str_free(char *string);
