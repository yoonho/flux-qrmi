# flux-qrmi

Flux is a next-generation, open-source resource management and scheduling framework designed to handle the complexities of modern supercomputing through hierarchical management, graph-based scheduling, and a modular architecture. Flux's hierarchical management means Flux can run inside Flux or other schedulers like Slurm and PBS. Flux provides graph-based scheduling through resource graphs that describe how CPUs, GPUs, memory, storage, etc are connected. Flux's modular architecture allows services to be easily swapped and extended.

Flux provides two plugin frameworks to support extensibility: jobtap and shell. Jobtap plugins extend the Flux scheduler. Shell plugins extend the task execution environment. The table below compares jobtap and shell plugins with respect to different aspects.

| Aspect     | Jobtap plugin                                 | Shell plugin                            |
| ---------- | --------------------------------------------- | --------------------------------------- |
| Layer      | Job manager (broker)                          | Job shell (execution)                   |
| Timing     | Before / during scheduling                    | During job execution                    |
| Scope      | Global / cluster-wide                         | Per job / per task                      |
| Purpose    | Control, policy, scheduling                   | Runtime environment, execution behavior |
| Runs where | Broker daemon                                 | Compute nodes / job context             |
| Analogy    | Slurm SPANK (scheduler side + submit plugins) | Slurm SPANK (task/exec side)            |

The initial QRMI integration focuses on using Flux as a "user-space" scheduler and running Quantum worklaods. The next step will extend the integration to Flux as the "system" scheduler and running hybrid Quantum-HPC workloads.

Shell plugins will provide sufficient functionality for the initial implementation. A jobtap plugin could be required in the future for Flux as the "system" scheduler and Quantum system scheduling. In the pbs-hooks-for-qrmi repository, @ohtanim compared Spank plugins with PBS hooks. The table below adds Flux shell plugins to the comparison.

|| Spank plugins | PBS Hooks | Shell plugins |
| ---- | ---- | ---- | ---- |
| Implementation language | C(shared library ```.so```) | Python(script) | C(shared library ```.so```) or Lua(script) |
| Deployment | Compiled ```.so``` distributed to each node manually | Registered via ```qmgr```; server distributes automatically | Compiled ```.so``` distributed to each node manually (can use Flux Arhive for large-scale distribution) |
| Execution location | ```slurmctld``` (controller) or ```slurmd``` (compute node), depending on context | PBS Server or MOM (compute node), selected per hook type | Flux Shell (compute node) |
| Hook/callback list | ```slurm_spank_init```, ```task_init```, ```task_exit```, etc. — ~10+ callbacks. Custom options via spank_option. | ```queuejob```, ```runjob```, ```execjob_begin```, ```execjob_end```, ```execjob_epilogue```, etc. — 20+ event types | ```shell.connect```, ```shell.init```, ```shell.post-init```, ```task.init```, ```task.exec```, ```task.fork```, ```task.exit```, ```shell.start```, ```shell.finish```, ```shell.exit``` |
| Job info access | SPANK API (```spank_get_item```, etc.) to read C structs | ```pbs.event().job``` object — attributes readable and writable directly | ```jobspec``` object |
| Job rejection / modification | [Limited] Return an error code to abort; modifying attributes is difficult | [Flexible] ```event.reject()``` and attribute rewriting are straightforward | ... |
| Environment variable passing | ```spank_setenv``` / ```spank_getenv``` for direct manipulation | Via ```job.Variable_List```; accessible as ```$PBS_VAR``` inside the job script | ```flux_shell_getenv``` / ```flux_shell_setenvf``` for direct manipulation |
| Custom resource integration | Add SPANK options to ```sbatch``` and combine with GRES, etc. | Define custom resources via ```qmgr```; specify in select or ```-l```; read inside the hook | Can extend the Flux resource graph |
| Logging / debugging | ```slurm_info```, ```slurm_error``` → slurmd log | ```pbs.logmsg()``` → PBS server log (configurable log level) | ```shell_log``` , ```shell_err```, etc |
| Code update | C source must be rebuilt and redeployed on every change | Re-register the updated Python script with qmgr | C source must be rebuilt and redeployed on every change, Lua script executed with each run/submit command |

The initial integration will follow the Spank and PBS pattern using shell.init and shell.exit plugins. Initially, resource configuration will pass through environment variables in the Flux shell. Later, qrmi_config.json support will be added. The shell.init plugin will set environment variables based on QRMI_QPU_RESOURCES, ie, prepend QPU name. The shell.init plugin will the acquire the QPUs and store acquisition tokens in environment variables. The shell.exit plugin will retrieve acquisition tokens from environment variables and release QPUs. Currently, the code in the repository is skeleton code.

Notes
* Should there be a system qrmi_config.json and a user qrmi_config.json?
* Two ways to set Flux shell environment variables
  * flux_shell_setenvf updates jobspec for all tasks and environments
  * flux_cmd_setenf updates one task enviroment, must be called in task.{init,exit}
* Other scheduler QRMI integrations
  * https://github.com/qiskit-community/spank-plugins - C plugins
    * Sampler example - https://github.com/qiskit-community/spank-plugins/blob/main/demo/qrmi/jobs/run_sampler.sh
    * Plugin - https://github.com/qiskit-community/spank-plugins/blob/main/plugins/spank_qrmi/spank_qrmi.c
  * https://github.com/ohtanim/pbs-hooks-for-qrmi - Python plugins
