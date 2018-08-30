# ARM Exception Model

ARMv8-A defines exception levels, from 0 to 3. They're called EL0 to EL3.
The bigger the exception level, the bigger the software execution priviledge.

EL0 is unpriviledged execution.
All implementations must support EL0 and EL1.

Execution can only move between levels by taking an exception or returning from one.
- Exception: level increases or remains the same.
             An exception cannot target EL0.
- Returning: level decreases or remains the same.

## Typical Exceltion level usage model.

- EL0: Applications.
- EL1: OS kernel and associated functions that are typically described as privileged.
- EL2: Hypervisor.
- EL3: Secure monitor.
