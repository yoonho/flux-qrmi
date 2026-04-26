#!/bin/bash

# Use environment variables via run/submit now, jobspec later
# ibm_marrakash_QRMI_IBM_QRS_ENDPOINT set by qrmi_shell plugin
source $HOME/.venv/bin/activate
python -c "import os, qrmi; print(\"$0: QRMI_QPU_RESOURCES \" + os.getenv('QRMI_QPU_RESOURCES','missing')); print(\"$0: ibm_marrakash_QRMI_IBM_QRS_ENDPOINT \" + os.getenv('ibm_marrakash_QRMI_IBM_QRS_ENDPOINT','missing'));"
#python $HOME/qrmi/examples/qiskit_primitives/ibm/sampler.py
