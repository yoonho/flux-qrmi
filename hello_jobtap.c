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
