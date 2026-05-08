## Install

### 1. Docker Build with Flux

First, create a container with Dockerfile.ubuntu24. You can use podman aliased to docker. The LLNL Flux container has ubuntu24.04 (noble) with python3.12. For testing, this was built on an AWS hpc7g (aarch64) instance.

```bash
# Build the initial container
docker build --network host -f Dockerfile.ubuntu24 -t ubuntu24-flux-qrmi:dev .
```

Note that the container above has been tagged and pushed if you want to use it (amd64 tag also available):

```bash
docker tag ubuntu24-flux-qrmi:dev ghcr.io/converged-computing/flux-qrmi:arm64
docker push ghcr.io/converged-computing/flux-qrmi:arm64
```

### 2. IBM Quantum Platform

Second, follow the steps below to get access to IBM Quantum Platform systems. This is free for a limited amount of quantum system time (10 mins per 28 days). Install instructions for the IBM client [are here](https://cloud.ibm.com/docs/cli?topic=cli-getting-started). You can do this locally, and the client is installed in the container (and you will need to authenticate, which is shown below).

 1. Create an account at https://quantum.cloud.ibm.com/
 2. Install the IBM Cloud CLI with `curl -fsSL https://clis.cloud.ibm.com/install/osx | sh` or `curl -fsSL https://clis.cloud.ibm.com/install/linux | sh`. You can also try to use the IBM Cloud dashboard to complete steps 3, 4, and 5.
 3. Login with `ibmcloud login`
 4. Create an API key with `ibmcloud iam api-key-create NAME -f FILE`. Your API key will be in FILE. Keep it in a safe place because you cannot retrieve it again.
 
Get the quantum systems service CRN with:

```bash
# See the output first
ibmcloud resource service-instances --service-name quantum-computing --output json | jq -r '.[] | {name: .name, crn: .crn}'

# Get the crn
IBM_SERVICE_CRN=$(ibmcloud resource service-instances --service-name quantum-computing --output json | jq -r '.[] | {name: .name, crn: .crn}' | jq -r .crn)
```

Get a list of available backends with the following commands:

```bash
export IAM_TOKEN=$(ibmcloud iam oauth-tokens | awk '{print $4}')
curl -X GET "https://quantum.cloud.ibm.com/api/v1/backends" \
-H "Authorization: Bearer $IAM_TOKEN" \
-H "Service-CRN: $IBM_SERVICE_CRN" \
-H "Accept: application/json"
```
```console
{"devices":["ibm_fez","ibm_marrakesh","ibm_kingston"]}
```

Note that the token generation using the cli did not work for me, but [the interface did](https://cloud.ibm.com/iam/apikeys).

## 3. Run a Quantum Job

We are going to shell into our container, use our token to authenticate, and then run a quantum job.

```bash
# Choose your platform of interest
docker run -it ghcr.io/converged-computing/flux-qrmi:amd64
docker run -it ghcr.io/converged-computing/flux-qrmi:arm64
```
```console
$ whoami
fluxuser
ƒ(s=1,d=0) fluxuser@45e368cd97ea:~$ which flux
/usr/bin/flux
ƒ(s=1,d=0) fluxuser@45e368cd97ea:~$ flux --version
commands:    		0.84.0-81-g2f0725ff9
libflux-core:		0.84.0-81-g2f0725ff9
libflux-security:	0.14.0
broker:  		0.84.0-81-g2f0725ff9
FLUX_URI:		local:///tmp/flux-p52t6V/local-0
build-options:		+hwloc.api==2.8.0+zmq==4.3.5
```

Login to the ibmcloud api.

```bash
ibmcloud login --apikey <your-api-key>
```

The Flux shell plugin is built and installed in the Dockerfile. You should find it at `/usr/lib/flux/shell/plugins/qrmi_shell.so`.

Let's again generate a backend, and then run a Flux job using it.

```bash
export IBM_APIKEY=<insert-your-key>
export IAM_TOKEN=$(ibmcloud iam oauth-tokens | awk '{print $4}')
export IBM_SERVICE_CRN=$(ibmcloud resource service-instances --service-name quantum-computing --output json | jq -r '.[] | {name: .name, crn: .crn}' | jq -r .crn)
```
```bash
IBM_BACKENDS=$(curl -X GET "https://quantum.cloud.ibm.com/api/v1/backends" \
-H "Authorization: Bearer $IAM_TOKEN" \
-H "Service-CRN: $IBM_SERVICE_CRN" \
-H "Accept: application/json")
```

Choose one...

```bash
IBM_BACKEND=$(echo $IBM_BACKENDS | jq -r .devices[1])
```

Build and install the Flux shell plugins and run a quantum job (this seems redundant to have to provide all credentials again):

```bash
flux run \
    --env=QRMI_QPU_RESOURCES="$IBM_BACKEND" \
    --env=QRMI_IBM_QRS_ENDPOINT="https://quantum.cloud.ibm.com/api/v1" \
    --env=QRMI_IBM_QRS_IAM_ENDPOINT="https://iam.cloud.ibm.com" \
    --env=QRMI_IBM_QRS_IAM_APIKEY="$IBM_APIKEY" \
    --env=QRMI_IBM_QRS_SERVICE_CRN="$IBM_SERVICE_CRN" \
    --env=QRMI_IBM_QRS_SESSION_MODE="batch" \
    ./run_sampler.sh
```

Right now, you will get a lot of debug output from "flux run". You can easily browse the [workload](https://quantum.cloud.ibm.com/workloads), which is recommended, since it might be PENDING for a while. Note that this example run takes 6s. From the sampler, you will see output like...

```bash
>>> Circuit ops (ISA): OrderedDict({'rz': 3030, 'sx': 753, 'rx': 511, 'cz': 378, 'measure': 127, 'barrier': 1})
>>> Job ID: d7okadm0b9ts73cig6a0
>>> Job Status: JobStatus.QUEUED
Counts for the 'meas' output register: {'0010001101011000011111111011111110011001010101000001001110010100011001100100011011111010101101000110101100000011101101101011000': 1, '1110011010101010111100101000010001101100001110111110000100000101000100100011011100101010000011010001100001110011001100010111100': 1, ...
```

Click on the workload to see scripts and diagrams. Cool!
