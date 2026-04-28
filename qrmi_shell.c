#define FLUX_SHELL_PLUGIN_NAME "qrmi_shell"
#include <flux/shell.h>
#include <stdlib.h>
#include "qrmi.h"

// Set environment variable in shell and task
int set_shell_task_env(flux_shell_t *shell, const char *name,
		       const char *value)
{
    if (flux_shell_setenvf(shell, 1, name, "%s", value) < 0) {
	shell_log("[%s] ERROR: flux_shell_setenvf failed for %s\n",
		  __func__, name);
	return -1;
    }
    if (setenv(name, value, 1) < 0) {
	shell_log("[%s] ERROR: setenv failed for %s\n", __func__, name);
	return -1;
    }
    return 0;
}

static int shell_init_cb(flux_plugin_t *p, const char *topic,
			 flux_plugin_arg_t *args, void *data)
{
    flux_shell_t *shell = flux_plugin_get_shell(p);
    // Assuming a single QPU resource
    const char *name = flux_shell_getenv(shell, "QRMI_QPU_RESOURCES");
    const char *endpoint =
	flux_shell_getenv(shell, "QRMI_IBM_QRS_ENDPOINT");
    const char *iam_endpoint =
	flux_shell_getenv(shell, "QRMI_IBM_QRS_IAM_ENDPOINT");
    const char *iam_apikey =
	flux_shell_getenv(shell, "QRMI_IBM_QRS_IAM_APIKEY");
    const char *service_crn =
	flux_shell_getenv(shell, "QRMI_IBM_QRS_SERVICE_CRN");
    const char *session_mode =
	flux_shell_getenv(shell, "QRMI_IBM_QRS_SESSION_MODE");

    if (setenv("QRMI_QPU_RESOURCES", name, 1) < 0) {
	shell_log("[%s] ERROR: setenv failed for name\n", __func__);
	return -1;
    }
    if (set_shell_task_env(shell, "SLURM_JOB_QPU_RESOURCES", name) < 0) {
	shell_log("[%s] ERROR: setenv failed for slurm name\n", __func__);
	return -1;
    }
    if (set_shell_task_env(shell, "SLURM_JOB_QPU_TYPES", "qiskit-runtime-service") < 0) {
	shell_log("[%s] ERROR: setenv failed for slurm qpu_types\n", __func__);
	return -1;
    }
    if (set_shell_task_env
	(shell, "ibm_marrakesh_QRMI_IBM_QRS_ENDPOINT", endpoint) < 0) {
	shell_log("[%s] ERROR: set_shell_task_env failed for endpoint\n",
		  __func__);
	return -1;
    }
    if (set_shell_task_env
	(shell, "ibm_marrakesh_QRMI_IBM_QRS_IAM_ENDPOINT",
	 iam_endpoint) < 0) {
	shell_log
	    ("[%s] ERROR: flux_shell_setenvf failed for iam_endpoint\n",
	     __func__);
	return -1;
    }
    if (set_shell_task_env
	(shell, "ibm_marrakesh_QRMI_IBM_QRS_IAM_APIKEY", iam_apikey) < 0) {
	shell_log("[%s] ERROR: flux_shell_setenvf failed for iam_apikey\n",
		  __func__);
	return -1;
    }
    if (set_shell_task_env
	(shell, "ibm_marrakesh_QRMI_IBM_QRS_SERVICE_CRN",
	 service_crn) < 0) {
	shell_log
	    ("[%s] ERROR: flux_shell_setenvf failed for service_crn\n",
	     __func__);
	return -1;
    }
    if (set_shell_task_env
	(shell, "ibm_marrakesh_QRMI_IBM_QRS_SESSION_MODE",
	 session_mode) < 0) {
	shell_log
	    ("[%s] ERROR: flux_shell_setenvf failed for session_mode\n",
	     __func__);
	return -1;
    }

    shell_log("[%s] getenv(QRMI_QPU_RESOURCES) %s\n", __func__,
	      getenv("QRMI_QPU_RESOURCES"));
    shell_log("[%s] getenv(SLURM_JOB_QPU_RESOURCES) %s\n", __func__,
	      getenv("SLURM_JOB_QPU_RESOURCES"));
    shell_log("[%s] getenv(SLURM_JOB_QPU_TYPES) %s\n", __func__,
	      getenv("SLURM_JOB_QPU_TYPES"));
    shell_log("[%s] ibm_marrakesh_QRMI_IBM_QRS_ENDPOINT %s\n", __func__,
	      flux_shell_getenv(shell,
				"ibm_marrakesh_QRMI_IBM_QRS_ENDPOINT"));
    shell_log("[%s] getenv(ibm_marrakesh_QRMI_IBM_QRS_ENDPOINT) %s\n",
	      __func__, getenv("ibm_marrakesh_QRMI_IBM_QRS_ENDPOINT"));
    shell_log("[%s] ibm_marrakesh_QRMI_IBM_QRS_IAM_ENDPOINT %s\n",
	      __func__, flux_shell_getenv(shell,
					  "ibm_marrakesh_QRMI_IBM_QRS_IAM_ENDPOINT"));
    shell_log("[%s] getenv(ibm_marrakesh_QRMI_IBM_QRS_IAM_ENDPOINT) %s\n",
	      __func__, getenv("ibm_marrakesh_QRMI_IBM_QRS_IAM_ENDPOINT"));
    shell_log("[%s] ibm_marrakesh_QRMI_IBM_QRS_IAM_APIKEY %s\n", __func__,
	      flux_shell_getenv(shell,
				"ibm_marrakesh_QRMI_IBM_QRS_IAM_APIKEY"));
    shell_log("[%s] getenv(ibm_marrakesh_QRMI_IBM_QRS_IAM_APIKEY) %s\n",
	      __func__, getenv("ibm_marrakesh_QRMI_IBM_QRS_IAM_APIKEY"));
    shell_log("[%s] ibm_marrakesh_QRMI_IBM_QRS_IAM_SERVICE_CRN %s\n",
	      __func__, flux_shell_getenv(shell,
					  "ibm_marrakesh_QRMI_IBM_QRS_SERVICE_CRN"));
    shell_log("[%s] getenv(ibm_marrakesh_QRMI_IBM_QRS_SERVICE_CRN) %s\n",
	      __func__, getenv("ibm_marrakesh_QRMI_IBM_QRS_SERVICE_CRN"));
    shell_log("[%s] ibm_marrakesh_QRMI_IBM_QRS_IAM_SESSION_MODE %s\n",
	      __func__, flux_shell_getenv(shell,
					  "ibm_marrakesh_QRMI_IBM_QRS_SESSION_MODE"));
    shell_log("[%s] getenv(ibm_marrakesh_QRMI_IBM_QRS_SESSION_MODE) %s\n",
	      __func__, getenv("ibm_marrakesh_QRMI_IBM_QRS_SESSION_MODE"));

    // _acquire_qpu from spank_qrmi.c in the spank_qrmi repo
    char *acquisition_token = NULL;
    bool is_accessible = false;
    QrmiReturnCode rc;
    const char *last_error = NULL;

    void *qrmi =
	qrmi_resource_new(name, QRMI_RESOURCE_TYPE_QISKIT_RUNTIME_SERVICE);
    if (qrmi == NULL) {
	last_error = qrmi_get_last_error();
	shell_log("[%s] ERROR: %s", __func__, last_error);
	qrmi_string_free((char *) last_error);
	return -1;
    }

    shell_log("[%s] qrmi %p", __func__, qrmi);
    rc = qrmi_resource_is_accessible(qrmi, &is_accessible);
    if ((rc != QRMI_RETURN_CODE_SUCCESS) || (is_accessible == false)) {
	last_error = qrmi_get_last_error();
	shell_log("[%s] ERROR: %s is not accessible. %s", __func__, name,
		  last_error);
	qrmi_string_free((char *) last_error);
	qrmi_resource_free(qrmi);
	return -1;
    }
    shell_log("[%s] %s is accessible", __func__, name);
    rc = qrmi_resource_acquire(qrmi, &acquisition_token);
    qrmi_resource_free(qrmi);
    if ((rc != QRMI_RETURN_CODE_SUCCESS) || (acquisition_token == NULL)) {
	last_error = qrmi_get_last_error();
	shell_log("[%s] ERROR: resource acquisition failed: %s. %s",
		  __func__, name, last_error);
	qrmi_string_free((char *) last_error);
	return -1;
    }

    if (setenv
	("ibm_marrakesh_QRMI_JOB_ACQUISITION_TOKEN", acquisition_token,
	 1) < 0) {
	shell_log("[%s] ERROR: setenv failed for acquisition_token\n",
		  __func__);
	return -1;
    }

    shell_log("[%s] acquisition_token: %s", __func__, acquisition_token);
    return 0;
}

static int task_init_cb(flux_plugin_t *p, const char *topic,
			flux_plugin_arg_t *args, void *data)
{
    shell_log("[%s] Hello\n", __func__);
    return 0;
}

static int shell_exit_cb(flux_plugin_t *p, const char *topic,
			 flux_plugin_arg_t *args, void *data)
{
    flux_shell_t *shell = flux_plugin_get_shell(p);
    const char *name = getenv("QRMI_QPU_RESOURCES");
    const char *acquisition_token =
	getenv("ibm_marrakesh_QRMI_JOB_ACQUISITION_TOKEN");

    if (name == NULL || acquisition_token == NULL) {
	shell_log
	    ("[%s] ERROR: QRMI_QPU_RESOURCES %s, ibm_marrakesh_QRMI_JOB_ACQUISITION_TOKEN %s\n",
	     __func__, name, acquisition_token);
	return -1;
    }

    shell_log("[%s] QRMI_QPU_RESOURCES %s\n", __func__, name);
    shell_log("[%s] ibm_marrakesh_QRMI_IBM_QRS_ENDPOINT %s\n", __func__,
	      flux_shell_getenv(shell,
				"ibm_marrakesh_QRMI_IBM_QRS_ENDPOINT"));
    shell_log("[%s] ibm_marrakesh_QRMI_IBM_QRS_IAM_ENDPOINT %s\n",
	      __func__, flux_shell_getenv(shell,
					  "ibm_marrakesh_QRMI_IBM_QRS_IAM_ENDPOINT"));
    shell_log("[%s] ibm_marrakesh_QRMI_IBM_QRS_IAM_APIKEY %s\n", __func__,
	      flux_shell_getenv(shell,
				"ibm_marrakesh_QRMI_IBM_QRS_IAM_APIKEY"));
    shell_log("[%s] ibm_marrakesh_QRMI_IBM_QRS_IAM_SERVICE_CRN %s\n",
	      __func__, flux_shell_getenv(shell,
					  "ibm_marrakesh_QRMI_IBM_QRS_SERVICE_CRN"));
    shell_log("[%s] ibm_marrakesh_QRMI_IBM_QRS_IAM_SESSION_MODE %s\n",
	      __func__, flux_shell_getenv(shell,
					  "ibm_marrakesh_QRMI_IBM_QRS_SESSION_MODE"));

    // _release_qpu from spank_qrmi.c in the spank_qrmi repo
    QrmiReturnCode rc;

    shell_log("[%s] releasing name(%s), type(%d), token(%s)", __func__,
	      name, QRMI_RESOURCE_TYPE_QISKIT_RUNTIME_SERVICE,
	      acquisition_token);
    void *qrmi =
	qrmi_resource_new(name, QRMI_RESOURCE_TYPE_QISKIT_RUNTIME_SERVICE);
    if (qrmi == NULL) {
	const char *last_error = qrmi_get_last_error();
	shell_log("[%s] ERROR: %s", __func__, last_error);
	qrmi_string_free((char *) last_error);
	return -1;
    }
    rc = qrmi_resource_release(qrmi, acquisition_token);
    if (rc != QRMI_RETURN_CODE_SUCCESS) {
	const char *last_error = qrmi_get_last_error();
	shell_log
	    ("[%s] ERROR: Failed to release acquired resource: name(%s), type(%d), token(%s), %s",
	     __func__, name, QRMI_RESOURCE_TYPE_QISKIT_RUNTIME_SERVICE,
	     acquisition_token, last_error);
	qrmi_string_free((char *) last_error);
    }
    rc = qrmi_resource_free(qrmi);
    if (rc != QRMI_RETURN_CODE_SUCCESS) {
	shell_log
	    ("[%s] ERROR: Failed to free QrmiQuantumResource handle: (%p)",
	     __func__, qrmi);
	return -1;
    }

    return 0;
}

int flux_plugin_init(flux_plugin_t *p)
{
    if (flux_plugin_add_handler(p, "shell.init", shell_init_cb, NULL) < 0
	|| flux_plugin_add_handler(p, "task.init", task_init_cb, NULL) < 0
	|| flux_plugin_add_handler(p, "shell.exit", shell_exit_cb,
				   NULL) < 0)
	return -1;

    return 0;
}
