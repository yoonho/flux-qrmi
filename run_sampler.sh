#!/bin/bash

# Use environment variables via run/submit now, jobspec later
# ibm_marrekash_QRMI_IBM_QRS_ENDPOINT set by qrmi_shell plugin
source $HOME/.venv/bin/activate
python -c "import os, qrmi; \
           print(\"$0: QRMI_QPU_RESOURCES \" + os.getenv('QRMI_QPU_RESOURCES','missing')); \
           print(\"$0: SLURM_JOB_QPU_RESOURCES \" + os.getenv('SLURM_JOB_QPU_RESOURCES','missing')); \
           print(\"$0: SLURM_JOB_QPU_TYPES \" + os.getenv('SLURM_JOB_QPU_TYPES','missing')); \
           print(\"$0: ibm_marrakesh_QRMI_IBM_QRS_ENDPOINT \" + os.getenv('ibm_marrakesh_QRMI_IBM_QRS_ENDPOINT','missing')); \
           print(\"$0: ibm_marrakesh_QRMI_IBM_QRS_IAM_ENDPOINT \" + os.getenv('ibm_marrakesh_QRMI_IBM_QRS_IAM_ENDPOINT','missing')); \
           print(\"$0: ibm_marrakesh_QRMI_IBM_QRS_IAM_APIKEY \" + os.getenv('ibm_marrakesh_QRMI_IBM_QRS_IAM_APIKEY','missing')); \
           print(\"$0: ibm_marrakesh_QRMI_IBM_QRS_SERVICE_CRN \" + os.getenv('ibm_marrakesh_QRMI_IBM_QRS_SERVICE_CRN','missing')); \
           print(\"$0: ibm_marrakesh_QRMI_IBM_QRS_SESSION_MODE \" + os.getenv('ibm_marrakesh_QRMI_IBM_QRS_SESSION_MODE','missing'));"
python $HOME/qrmi/examples/qiskit_primitives/ibm/sampler.py
