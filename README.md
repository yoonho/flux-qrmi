# flux-hooks-for-qrmi

Other QRMI "ports"
* https://github.com/qiskit-community/spank-plugins - C plugins
  * Sampler example - https://github.com/qiskit-community/spank-plugins/blob/main/demo/qrmi/jobs/run_sampler.sh
  * Plugin - https://github.com/qiskit-community/spank-plugins/blob/main/plugins/spank_qrmi/spank_qrmi.c
* https://github.com/ohtanim/pbs-hooks-for-qrmi - Python plugins

Use Dockerfile to add network to flux-sched container

    docker build -t flux-sched-net:latest .
    # Start Flux, cap-add is needed for ping
    docker run -it --cap-add=NET_RAW flux-sched-net:latest
    # Do not start Flux
    docker run -it --cap-add=NET_RAW --entrypoint bash flux-sched-net:latest

How to add a resource

    # Get current resources
    flux resource list
    # Get resource graph json
    flux resource R > backup.json
    # Remove Fluxion scheduler
    flux module remove sched-fluxion-qmanager; flux module remove sched-fluxion-resource; flux module remove sched-fluxion-feasibility
    # Reload resource graph json
    flux resource reload backup.json
    # Can modify backup.json, eg, add gpu
    # Reload Fluxion scheduler
    flux module load sched-fluxion-resource; flux module load sched-fluxion-qmanager; flux module load sched-fluxion-feasibility
