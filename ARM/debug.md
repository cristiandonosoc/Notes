# Debug

From the POV of the arch, a debugger are the OS or software that handles debug exceptions.

## Context ID

CONTEXTIDR_ELx are registers that identifies the current ContextID.
There is only CONTEXTIDR_EL1 and CONTEXTIDR_EL2.

The registers consists of a 32-bit variable that describes the PID (Process ID).

## Debug Exceptions

There are 4 kinds of exceptions, that need to be activated:

### Breakpoint Instruction Exceptions:

Instructions that generate exceptions.
Used to create _Software Breakpoints_.

## Breakpoint Exceptions:

ARMv8-A provides 2-16 hardware breakpoints registers.
The number is obtained by the BRPs field in the EDDFR register.

They generate exceptions on execution of a particular address.

There are two kind of breakpoints:
- Address: Execution of a particular virtual address.
- Context:
  - Context ID: Whenever the execution matches the PID in CONTEXTIDR_EL1 (?)
  - VMID: Whenever the VMID in VTTBR_EL2 (?)
  - ContextID and VMID match.

  The usage of context is that an address breakpoint can point to a context/vm
  breakpoint, and only trigger is it matches, thus setting hardware breakpoints only
  to a particular VM/process.

### Wathpoint Exceptions:

ARMv8-A provides 2-16 watchpoint breakpoint registers.
The number is obtained by the WRPs field in the EDDFR register.


