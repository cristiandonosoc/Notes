# ptrace

Linux interface for managing debug-wise to another process.
Requires permissions.

PTRACE_SETREGSET

## In latest kernel (4.17.12)

#### kernel/ptrace.c:1110

Defines a generic ptrace call.
- First checks ofr TRACEME, ATTACH, SEIZE, KILL, DETACH, INTERRUPT.
-- Those are generic events that requires checks.

Then calls arch_ptrace, which is defined architecture dependent.

### Arm64

#### arch/arm64/kernel/ptrace.c:1603

arch_ptrace passes to ptrace_request.

#### kernel/ptrace.c:883

Switch/dispatcher for the PTRACE_* options.

PTRACE_GETREGSET/PTRACE_SETREGSET
Creates the iovec vectors.
Calls ptrace_regset


call trace

kernel/ptrace.c:1110 -> SYSCALL ptrace
arch/arm64/kernel/ptrace.c:1603 -> arch_ptrace
kernel/ptrace.c:883-> ptrace_request
kernel/ptrace.c:853 -> ptrace_regset
include/linux/regset.h -> copy_regset_from_user










