#include <flux/core.h>
#include <flux/jobtap.h>
#include <jansson.h>

/* Helper function to get environment variable from jobspec */
static const char *get_job_env(flux_t *h, json_t *jobspec, const char *var_name)
{
    json_t *attrs, *system, *environment, *var;

    attrs = json_object_get(jobspec, "attributes");
    if (!attrs)
        return NULL;
    flux_log(h, LOG_INFO, "Hello! Found attributes");

    // Access from user attributes
    json_t *user = json_object_get(attrs, "user");
    if (!user) return NULL;
    json_t *env = json_object_get(user, "env");
    if (!env) return NULL;
    json_t *my_var = json_object_get(env, "MY_VAR");
    if (!my_var) return NULL;

    if (my_var && json_is_string(my_var)) {
        const char *value = json_string_value(my_var);
        flux_log(h, LOG_INFO, "Hello! MY_VAR=%s", value);
    }

    system = json_object_get(attrs, "system");
    if (!system)
        return NULL;
    flux_log(h, LOG_INFO, "Hello! Found system");

    environment = json_object_get(system, "environment");
    if (!environment)
        return NULL;
    flux_log(h, LOG_INFO, "Hello! Found environment");

    var = json_object_get(environment, var_name);
    if (!var || !json_is_string(var))
        return NULL;

    flux_log(h, LOG_INFO, "Hello! Found %s", var_name);
    return json_string_value(var);
}

static int validate_cb(flux_plugin_t *p,
                       const char *topic,
                       flux_plugin_arg_t *args,
                       void *data)
{
    flux_jobid_t id;
    json_t *jobspec = NULL;
    flux_t *h = flux_jobtap_get_flux(p);

    // Get environment variables

    if (flux_plugin_arg_unpack(args, FLUX_PLUGIN_ARG_IN,
                                "{s:I s:o}", "id", &id, "jobspec", &jobspec) < 0)
        return -1;

    const char *endpoint = get_job_env(h, jobspec, "QRMI_IBM_QRS_ENDPOINT");

    flux_log(h, LOG_INFO, "Hello! Job %ju was submitted. QRMI_IBM_QRS_ENDPOINT %s", (uintmax_t)id, endpoint);

    return 0;

#if 0
    // Hardcode configuration???
    char* name = "ibm_marrakesh";
    QrmiResourceType type = QRMI_RESOURCE_TYPE_QISKIT_RUNTIME_SERVICE;
"QRMI_IBM_QRS_ENDPOINT": "https://quantum.cloud.ibm.com/api/v1",
"QRMI_IBM_QRS_IAM_ENDPOINT": "https://iam.cloud.ibm.com",
"QRMI_IBM_QRS_IAM_APIKEY": "AOEQdiezBVEdDpD3Qwz75j6pXpKFhxEf9nauu6jjDusA",
"QRMI_IBM_QRS_SERVICE_CRN": "crn:v1:bluemix:public:quantum-computing:us-east:a/7a0a34127e6341618b52a2ec661054f5:ff0622bf-43ac-4cd0-994d-2a139cf81608::",
"QRMI_IBM_QRS_SESSION_MODE": "batch"

    char *acquisition_token = NULL;
    bool is_accessible = false;
    QrmiReturnCode rc;
    const char *last_error = NULL;

    void *qrmi = qrmi_resource_new(name, type);
    if (qrmi == NULL) {
        last_error = qrmi_get_last_error();
        slurm_error("%s, %s", plugin_name, last_error);
        spank_setenv(spank_ctxt, "QRMI_PLUGIN_ERROR", last_error, KEEP_IF_EXISTS);
        qrmi_string_free((char *)last_error);
        return NULL;
    }

    slurm_debug("%s, qrmi: %p", plugin_name, qrmi);
    rc = qrmi_resource_is_accessible(qrmi, &is_accessible);
    if ((rc != QRMI_RETURN_CODE_SUCCESS) || (is_accessible == false)) {
        last_error = qrmi_get_last_error();
        slurm_error("%s, %s is not accessible. %s", plugin_name, name, last_error);
        spank_setenv(spank_ctxt, "QRMI_PLUGIN_ERROR", last_error, KEEP_IF_EXISTS);
        qrmi_string_free((char *)last_error);
        qrmi_resource_free(qrmi);
        return NULL;
    }
    rc = qrmi_resource_acquire(qrmi, &acquisition_token);
    qrmi_resource_free(qrmi);
    if ((rc != QRMI_RETURN_CODE_SUCCESS) || (acquisition_token == NULL)) {
        last_error = qrmi_get_last_error();
        slurm_error("%s, resource acquisition failed: %s. %s", plugin_name, name, last_error);
        spank_setenv(spank_ctxt, "QRMI_PLUGIN_ERROR", last_error, KEEP_IF_EXISTS);
        qrmi_string_free((char *)last_error);
        return NULL;
    }

    slurm_debug("%s, acquisition_token: %s", plugin_name, acquisition_token);
    return _acquired_resource_create(name, type, acquisition_token);
#endif
}

static int run_cb(flux_plugin_t *p,
                  const char *topic,
                  flux_plugin_arg_t *args,
                  void *data)
{
    flux_jobid_t id;
    flux_t *h = flux_jobtap_get_flux(p);

    if (flux_plugin_arg_unpack(args, FLUX_PLUGIN_ARG_IN,
                                "{s:I}", "id", &id) < 0)
        return -1;

    flux_log(h, LOG_INFO, "Hello! Job %ju is now running", (uintmax_t)id);

    return 0;
}

static int cleanup_cb(flux_plugin_t *p,
                      const char *topic,
                      flux_plugin_arg_t *args,
                      void *data)
{
    flux_jobid_t id;
    flux_t *h = flux_jobtap_get_flux(p);

    if (flux_plugin_arg_unpack(args, FLUX_PLUGIN_ARG_IN,
                                "{s:I}", "id", &id) < 0)
        return -1;

    flux_log(h, LOG_INFO, "Hello! Job %ju is now finished", (uintmax_t)id);

    return 0;
}

static const struct flux_plugin_handler tab[] = {
    { "job.validate", validate_cb, NULL },
    { "job.state.run", run_cb, NULL },
    { "job.state.cleanup", cleanup_cb, NULL },
    { 0 }
};

int flux_plugin_init(flux_plugin_t *p)
{
    flux_log(flux_jobtap_get_flux(p), LOG_INFO,
             "Hello jobtap plugin loaded!");

    return flux_plugin_register(p, "hello", tab);
}
