#define FLUX_SHELL_PLUGIN_NAME "qrmi_shell"
#include <flux/shell.h>
#include <stdlib.h>

static int shell_init_cb (flux_plugin_t *p,
                          const char *topic,
                          flux_plugin_arg_t *args,
                          void *data)
{
    flux_shell_t *shell = flux_plugin_get_shell (p);

    shell_log("[shell_init_cb] QRMI_QPU_RESOURCES %s\n", flux_shell_getenv (shell, "QRMI_QPU_RESOURCES"));
    if (flux_shell_setenvf (shell, 1, "ibm_marrakash_QRMI_IBM_QRS_ENDPOINT", "%s", flux_shell_getenv (shell, "QRMI_IBM_QRS_ENDPOINT")) < 0)
    {
        printf("[shell_init_cb] flux_shell_setenvf failed\n");
        return -1;
    }
    shell_log("[shell_init_cb] ibm_marrakash_QRMI_IBM_QRS_ENDPOINT %s\n", flux_shell_getenv (shell, "ibm_marrakash_QRMI_IBM_QRS_ENDPOINT"));
    
    return 0;
#if 0
    // _acquire_qpu from spank_qrmi.c in the spank_qrmi repo
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

static int task_init_cb (flux_plugin_t *p,
                         const char *topic,
                         flux_plugin_arg_t *args,
                         void *data)
{
    shell_log("[task_init_cb] Hello\n");
    
    return 0;
}

static int shell_exit_cb (flux_plugin_t *p,
                          const char *topic,
                          flux_plugin_arg_t *args,
                          void *data)
{
    shell_log("[shell_exit_cb] Hello\n");
    
    return 0;
}

int flux_plugin_init (flux_plugin_t *p)
{
    if (flux_plugin_add_handler (p, "shell.init", shell_init_cb, NULL) < 0
        || flux_plugin_add_handler (p, "task.init", task_init_cb, NULL) < 0
        || flux_plugin_add_handler (p, "shell.exit", shell_exit_cb, NULL) < 0)
        return -1;
    
    return 0;
}
