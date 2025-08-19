#include <qiskit.h>

typedef struct Service Service;
typedef struct Job Job;
typedef struct Backend Backend;
typedef struct BackendSearchResults BackendSearchResults;

extern int32_t qkrt_service_new(Service **out);

extern void qkrt_service_free(Service *service);

extern int32_t qkrt_backend_search(BackendSearchResults **out, Service *service);

extern void qkrt_backend_search_results_free(BackendSearchResults *results);

extern uint64_t qkrt_backend_search_results_length(BackendSearchResults *results);

extern Backend** qkrt_backend_search_results_data(BackendSearchResults *results);

extern Backend* qkrt_backend_search_results_least_busy(BackendSearchResults *results);

extern QkTarget* qkrt_get_backend_target(Service *service , Backend *backend); 

extern char* qkrt_backend_name(Backend *backend);

extern char* qkrt_backend_instance_crn(Backend *backend);

extern char* qkrt_backend_instance_name(Backend *backend);

extern int32_t qkrt_sampler_job_run(Job **out, Service *service, Backend *backend, QkCircuit *circuit, int32_t shots, char *runtime);

extern int32_t qkrt_job_status(uint32_t *out, Service *service, Job *job);

extern void qkrt_job_free(Job *job);

extern void generate_qpy(QkCircuit *circuit, char *filename);
