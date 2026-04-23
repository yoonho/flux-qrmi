#include <flux/core.h>
#include <flux/jobtap.h>

static int validate_cb(flux_plugin_t *p,
                       const char *topic,
                       flux_plugin_arg_t *args,
                       void *data)
{
    flux_jobid_t id;
    flux_t *h = flux_jobtap_get_flux(p);

    if (flux_plugin_arg_unpack(args, FLUX_PLUGIN_ARG_IN,
                                "{s:I}", "id", &id) < 0)
        return -1;

    flux_log(h, LOG_INFO, "Hello! Job %ju was submitted", (uintmax_t)id);

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
