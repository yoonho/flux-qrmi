# flux-hooks-for-qrmi

Other QRMI "ports"
* https://github.com/qiskit-community/spank-plugins - C plugins
  * Sampler example - https://github.com/qiskit-community/spank-plugins/blob/main/demo/qrmi/jobs/run_sampler.sh
  * Plugin - https://github.com/qiskit-community/spank-plugins/blob/main/plugins/spank_qrmi/spank_qrmi.c
* https://github.com/ohtanim/pbs-hooks-for-qrmi - Python plugins

Jobtap plugin vs Shell plugin

| Aspect     | Jobtap plugin                                 | Shell plugin                            |
| ---------- | --------------------------------------------- | --------------------------------------- |
| Layer      | Job manager (broker)                          | Job shell (execution)                   |
| Timing     | Before / during scheduling                    | During job execution                    |
| Scope      | Global / cluster-wide                         | Per job / per task                      |
| Purpose    | Control, policy, scheduling                   | Runtime environment, execution behavior |
| Runs where | Broker daemon                                 | Compute nodes / job context             |
| Analogy    | Slurm SPANK (scheduler side + submit plugins) | Slurm SPANK (task/exec side)            |

Can pass environment variables to jobtap plugins with --setattr

    flux submit --setattr=user.env.MY_VAR=test hostname

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
