# HW Breakpoint Proposal

--------------------------------------------------------------------------------

This document outlines how zxdb should implement hw breakpoints and what support
is needed from zircon.

## Other Debuggers

### GDB

For each particular process, gdb keeps track of the debug "state". It has both
generic and arch dependent info. This is tracked server side.
For arm64, the tracking is actually only the hw debug state:

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

The tracked register information is quite straighforward. Interestingly enough,
apparently gdb doesn't keeps track whether a particular hw breakpoint is
per-thread, so instead will apply it to all the threads of the process and
leave the interpretation of the breakpoint event to the client.

The "installation" of the breakpoint occurs when gdb resumes a thread. Before
restarting gdb will query the OS for the debug registers and check the diff
between those and the state gdb thinks it should be. It will then apply the
diff into the reg state. After that the debug register is set.

Then, if the breakpoint is triggered, the server checks what kind of event it was
and reacts accordingly. There is a lot of handling of effects, particularly for
watchpoints: If there was a data-watchpoint hit, this means that the write to the
memory has not happened and the data will need to be written.

## Proposal

### ZXDB tracking

zxdb already has infrastructure for software breakpoints and it was made with the hw
extension in mind. A generic debug state will have to be placed so that the debug_agent
can refer to them. The concepts that it needs to support are:

- HW breakpoints
- Watchpoints: Read/Write/(JUMP?)

After we have that setup, we need to expose the functionality into the user:

- New debug_ipc commands?
- New verbs?
- How to show this information in the UI?

### Runtime

I would like to follow the same approach as gdb, only with a little more fine-grained
information. GDB doesn't keep track of which thread requested a breakpoint, but in zxdb
we have that information.

So the same approach of updating the register information will still be used, but only
for the threads that effectivelly require it.

Q: What happens on first breakpoint installation? Will the process/thread require to
be stopped? *study how zxdb does it for sw breakpointw*.

### Zircon support

This feature will require zircon support for the register handling and (optionally)
could use support for un-eventful stepping.

The register state needs to be accesible for zxdb. This currently work in
zx_read_thread_state
zx_write_thread_state

These syscalls use the concept of register sets. Each register set points to a
particular category of registers which are represented in a specific struct that
containes them. These struct are architecture specific and are defined in
syscalls/debug.h (*FIND CORRECT LOCATION*).

I propose adding a new register set *ZX_DEBUG_REGISTER_SET* (See correct naming).
This will come with the following structs (*FLUSH THE REGISTER PER ARCH*).

The state scrapping will have to be updated, because currently zircon does not
keep track of this information in the internal thread state tracking.

*EXPLAIN WHERE THE THREAD STATE SCRAPING WORK AND WHERE AND WHAT CHANGEW NEED TO BE DONE*

This is the strict minimum support zxdb needs to get hw breakpoints running.

#### Optional support

Some architecture/OS combos give a functionality to step in a thread ignoring hw watchpoints,
this is not in the scope of this initial setup.

Optionally, it could be interesting to see the possibility of implementing event-less
stepping. This is because some hw breakpoints, specifically watchpoints, require to step
the thread in order to show the correct state to the user.

A write WP will stop *before* the write happened, meaning that the thread needs to be stepped
once in order to get the correct value. Normally this would require to uninstall the hw
breakpoint register state, step the thread and then reinstall the hw breakpoint.



## Structs

struct PerThreadHWBreakpointSetup {
  // All the addresses that have been installed within this thread.
  std::vector<uint64_t> installed_addrs;
};
