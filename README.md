# flux-hooks-for-qrmi

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
