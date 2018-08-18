# GDB hardware breakpoint tracking

GDB uses an important struct to track the information about a process: `process_info`.
Defined in `gdb/gdbserver/inferiors.h` (and inferior is the term for the process being
debugged by the gdb server).

Maintains the relevant information (pid, whether it's attached) and a set of pointers
to lists (breakpoints, tracepoints).
Of course, as this tracks processes which can vary a lot from arch to arch, somewhere
there is going to be a forward declared pointer to a struct that will be defined per
architecture. This struct is

process_info -> process_info_private -> arch_process_info

Apparently, they tend to be defined in `gdb/gdbserver/linux-<ARCH>-low.c` (at least for ARM they are).

## ARMv8 tracking

the ARM arch_process_info only holds debug_reg_state, which seems to handle only
hardware breakpoint state:

```
struct arch_process_info {
  /* Hardware breakpoint/watchpoint data.
     The reason for them to be per-process rather than per-thread is
     due to the lack of information in the gdbserver environment;
     gdbserver is not told that whether a requested hardware
     breakpoint/watchpoint is thread specific or not, so it has to set
     each hw bp/wp for every thread in the current process.  The
     higher level bp/wp management in gdb will resume a thread if a hw
     bp/wp trap is not expected for it.  Since the hw bp/wp setting is
     same for each thread, it is reasonable for the data to live here.
     */
  struct aarch64_debug_reg_state debug_reg_state;
};
```

The comment is actually very telling about the breakpoint handling for this architecture,
and it would seem that it also applies to all architectures. Apparently gdbserver has no
way to tell if a particular hw breapoint/watchpoint applies to a specific thread, but
instead that information is handled in the client. So gdbserver will apply the breakpoint
to every thread on the process and the client will decide if the thread should continue
or not.

GDB uses struct `aarch64_debug_reg_state` for tracking hw breakpoint/watchpoints

```
struct aarch64_debug_reg_state
{
  /* hardware breakpoint */
  CORE_ADDR dr_addr_bp[AARCH64_HBP_MAX_NUM];              // HW BP address.
  unsigned int dr_ctrl_bp[AARCH64_HBP_MAX_NUM];           // Control register context.
  unsigned int dr_ref_count_bp[AARCH64_HBP_MAX_NUM];      // Reference count.

  /* hardware watchpoint */
  /* Address aligned down to AARCH64_HWP_ALIGNMENT.  */
  CORE_ADDR dr_addr_wp[AARCH64_HWP_MAX_NUM];              // WP address.
  /* Address as entered by user without any forced alignment.  */
  CORE_ADDR dr_addr_orig_wp[AARCH64_HWP_MAX_NUM];         // Original trackpoint (can differ from addr).
  unsigned int dr_ctrl_wp[AARCH64_HWP_MAX_NUM];           // Control register context.
  unsigned int dr_ref_count_wp[AARCH64_HWP_MAX_NUM];      // Reference count.
};
```

Also tracks per lwp (thread) context.

```
struct arch_lwp_info
{
  /* When bit N is 1, it indicates the Nth hardware breakpoint or
     watchpoint register pair needs to be updated when the thread is
     resumed; see aarch64_linux_prepare_to_resume.  */
  dr_changed_t dr_changed_bp;
  dr_changed_t dr_changed_wp;
};
```

This last struct is used for when the thread is resumed, in which the hw register needs
to be updated.
So when the breakpoints are set (in gdb/nat/aarch64-linux-hw-point.c:debug_reg_change_callback),
the breakpoints will be marked as dirty and will be updated the next time
the thread is resumed.

### Thread restarting

Once we have determined which hw breakpoints/watchpoints we want to track, we have to actually
set the machinery in the CPU state. This is done by setting the correct values in the corresponding
debug registers.

That will trigger a per-thread exception which gdb will have to interpret later.
This is a key concept that is to undertand. The CPU and the debug server has very little
context about what it's doing, and in turn it exposes a somewhat "simple" API:
Tell me which threads to attach/stop/set registers and I'll let you know what happens.
For example, to put a process wide breakpoint, that becomes a per-thread breakpoint for
each thread, and then the reaction of what happens for one of them has to be done by the server,
and sometimes even by the client. The CPU has no concept of a process.

Keep in mind that even though gdb tracks all the information, it doesn't have access to the
debugged process/thread registers. It has to go through the API that the OS exposes. In the case
of Linux, this is ptrace, the overall multiplexed, all encompasing call that enables you to
control a debugged process. In this sense at least, GDB is a very convoluted wrapper over ptrace.

The actual call process is somewhat convoluted and always forget it, but it goes something like
this:

gdb/linux-nat.c:linux_nat_target::resume
  gdb/linux-nat.c:linux_nat_resume_callback
    gdb/linux-nat.c:resume_lwp
      gdb/linux-nat.c:linux_resume_one_lwp
        gdb/linux-nat.c:linux_target->low_prepare_to_resume
          <<ARCH_DEPENDENT>>
          gdb/nat/aarch64-linux.c:aarch64_linux_nat_target::low_prepare_to_resume
            gdb/nat/aarch64_linux.c:aarch64_linux_prepare_to_resume
              gdb/gdbserver/linux-aarch64-low.c:aarch64_debug_reg_state
                // Get the internal register representation.
                // Mainly the hw debug registers.
              // For each hw debug register that has changed
              gdb/nat/aarch64-linux-hw-point.c:aarch64_linux_set_debug_regs
                // Calls ptrace with PTRACE_SET_REGSET
        gdb/linux-nat.c:linux_target->low_resume
          <<ARCH_DEPENDENT>>
          inf_ptrace_target::resume
            // Does the actual ptrace call to resume the thread

With that we have a running thread that can will stop correctly with the correspondent
hardware registers set. Other architectures will do something similar.


