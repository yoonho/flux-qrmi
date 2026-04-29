First, create a container with Dockerfile.ubuntu24. I use podman aliased to docker. I was not able to use the LLNL Flux container (fluxrm//flux-sched:latest) due to an emulation error from Rust. The LLNL Flux container is amd64 and not aarch64. Also, starting from ubuntu24.04 makes more sense because it contains python3.12. The LLNL Flux container is ubuntu22.04 with python3.10 and python3.11.

    # Development convenience
    cp -r $HOME/.git .
    cp -r $HOME/.ssh .
    # Increase podman memory to 16GB for Spack/Flux
    docker machine stop
    docker machine set --memory 16384
    docker machine start
    # Build the initial container
    docker build -f Dockerfile.ubuntu24 -t ubuntu24:latest .
    # After docker run, you can use ctrl-P ctrl-Q to exit without stopping container
    docker run -it --cap-add=NET_RAW --name ubuntu24 ubuntu24:latest
    # To get another shell
    docker exec -it ubuntu24 /bin/bash
    # Get spack
    git clone https://github.com/spack/spack.git /home/fluxuser/spack
    source /home/fluxuser/spack/share/spack/setup-env.sh # Add to .bashrc for future logins
    spack compiler find
    spack install flux-sched +docs # About 20 mins - go get some coffee
    spack load flux-sched # Add to .bashrc for future logins
    # Make sure Flux is installed
    flux --version
    # Install and build the QRMI Rust/C API library
    git clone git@github.com:qiskit-community/qrmi.git
    cd qrmi
    . $HOME/.cargo/env # Add to .bashrc for future logins
    cargo clean
    cargo build --locked --release
    # Check for $HOME/qrmi/qrmi.h and $HOME/qrmi/target/release/libqrmi.a
    # Install QRMI Python API library via PyPI (thank you, Claudio!)
    cd $HOME
    python3 -m venv .venv
    source $HOME/.venv/bin/activate
    pip install --upgrade pip
    pip install qrmi
    # Test install
    python3 -c "import qrmi; print(\"Hello\");"
    # Good time to commit changes, for example
    docker commit ubuntu24-flux-qrmi:dev

Second, follow the steps below to get access to IBM Quantum Platform systems. This is free for a limited amount of quantum system time (10 mins per 28 days).

 1. Create an account at https://quantum.cloud.ibm.com/
 2. Install the IBM Cloud CLI with `curl -fsSL https://clis.cloud.ibm.com/install/osx | sh`. You can also try to use the IBM Cloud dashboard to complete steps 3, 4, and 5.
 3. Login with `ibmcloud login`
 4. Create an API key with `ibmcloud iam api-key-create NAME -f FILE`. Your API key will be in FILE. Keep it in a safe place because you cannot retrieve it again.
 5. Get the quantum systems service CRN with `ibmcloud resource service-instances --service-name quantum-computing --output json | jq -r '.[] | {name: .name, crn: .crn}'`.
 6. Get a list of available backends with the following commands:
 
    export IAM_TOKEN=$(ibmcloud iam oauth-tokens | awk '{print $4}')
    curl -X GET "https://quantum.cloud.ibm.com/api/v1/backends" \
    -H "Authorization: Bearer $IAM_TOKEN" \
    -H "Service-CRN: YOUR_SERVICE_CRN_FROM_STEP_5" \
    -H "Accept: application/json"

Build and install the Flux shell plugins and run a quantum job

    git clone git@github.com:yoonho/flux-qrmi.git
    cd flux-qrmi
    make; make install
    flux run --env=QRMI_QPU_RESOURCES="YOUR_BACKEND_FROM_STEP_6" \
             --env=QRMI_IBM_QRS_ENDPOINT="https://quantum.cloud.ibm.com/api/v1" \
             --env=QRMI_IBM_QRS_IAM_ENDPOINT="https://iam.cloud.ibm.com" \
             --env=QRMI_IBM_QRS_IAM_APIKEY="YOUR_API_KEY_FROM_STEP_4" \
             --env=QRMI_IBM_QRS_SERVICE_CRN="YOUR_SERVICE_CRN_FROM_STEP_5" \
             --env=QRMI_IBM_QRS_SESSION_MODE="batch" \
             ./run_sampler.sh
   
Right now, you will get a lot of debug output from "flux run". From the sampler, you will see output like...
   
    >>> Circuit ops (ISA): OrderedDict({'rz': 3030, 'sx': 753, 'rx': 511, 'cz': 378, 'measure': 127, 'barrier': 1})
    >>> Job ID: d7okadm0b9ts73cig6a0
    >>> Job Status: JobStatus.QUEUED
    Counts for the 'meas' output register: {'0010001101011000011111111011111110011001010101000001001110010100011001100100011011111010101101000110101100000011101101101011000': 1, '1110011010101010111100101000010001101100001110111110000100000101000100100011011100101010000011010001100001110011001100010111100': 1, ...
