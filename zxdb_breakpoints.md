# ZXDB breakpoints

All the communication occurs the debug_ipc, which is a message based protocol.

On the target, the `debug_agent` binary handles the process/thread debugging.
Works as a server for debug_ipc commands/events.

## Breakpoint registering

Breakpoints get registered to a process. That means that each breakpoint
registering call gets handled by the `DebugProcess`.

## debug_agent::DebuggedProcess (debugged_process.h)

A debugged process is pretty much what you expect:
- PID.
- A handle to the underlying OS process handle.
- Map of threads keyed by TID.
- A Map of ProcessBreakpoints, keyed by address.

So registering a Breakpoint means looking if a ProcessBreakpoint is already
set on the address and either update it or add a new one.
It's important to update it because, even though rare, you could have two
breakpoints on the same location, and you want to notify on both.

## debug_agent::Breakpoint (breakpoint.h)

A Breakpoint is the logical concept of a breakpoint (eg. b main).
The Breakpoint doesn't handle the memory breakpoint directly, those are handled
by the breakpoint locations.

A Breakpoint can have many locations, and it's not an edge case:
- Templated functions.
- Functions that get inlined.

In terms of actual handling of the breakpoint registering, the breakpoint
delegates to the amptly named `debug_ipc::Breakpoint::ProcessDelegate.
That class handle the actual implementation of the register/unregister.
Using a delegate enables us to being able to mock the dependency.

Right now, besides testing, the ProcessDelegate is the DebugAgent itself,
which makes sense, as it has all the information of the system.

## debug_agent::ProcessBreakpoint (process_breakpoint.h)

So a process breakpoint is a particular instantiation of a Breakpoint
in an individual address of a process. That means that a Breakpoint can
have many ProcessBreakpoint associated to it.

Interestingly, even though there is a clear owning relationship between
Breakpoint and ProcessBreakpoint, the actual relationship is many to many.
In order to not duplicate ProcessBreakpoints, each ProcessBreakpoint has a
reference to all the owninig Breakpoints.

So registering a breakpoint is just adding that Breakpoint to the list
of owning breakpoints for the ProcessBreakpoint.

### debug_agent::ProcessBreakpoint::Init

A ProcessBreakpoint must be initialized. That is the actual logic that installs
the breakpoint within the system.

Right now, ProcessBreakpoint represent a software breakpoints. That means that
when a ProcessBreakpoint is installed, it replaces the requested address with
the break instruction, which is architecture dependent:

- x64: INT3 (0xCC) -> Generate breakpoint trap
- arm64: BRK 0
         110x 0100 xx1i iiii iiii iiii iiix xx00 - i is the argument (0 in this case).

The debugger has to keep track of what instruction was there in order to be able
to replace it so that the program can continue corretly after this.

## Breakpoint Hit cycle

DebuggedProcess::OnException (because it's a ZirconExceptionWatcher)
  // An exception is bounded to a thread id
  DebuggedThread::OnException
    IF SOFTWARE BREAKPOINT {
      DebuggedThread::UpdateForSoftwareBreakpoint
        DebuggedThread::UpdateForHitProcessBreakpoint
          ProcessBreakpoint::OnHit
            Breakpoint::OnHit
    }
    IF HARDWARE BREAKPOINT
      << TODO: Complete HW breakpoint story >>





### debug_agent::ProcessBreakpoint::OnHit

The actual hit handling is very simple: just see how many breakpoints are associated
and call the OnHit handler.
The handler receives a BreakpointStats entry (part of the protocol, in order to communicate
back to the client what happened:

```
struct BreakpointStats {
  uint32_t breakpoint_id = 0;
  uint32_t hit_count = 0;

  // On a "breakpoint hit" message from the debug agent, if this flag is set,
  // the agent has deleted the breakpoint because it was a one-shot breakpoint.
  // Whenever a client gets a breakpoint hit with this flag set, it should
  // clear the local state associated with the breakpoint.
  bool should_delete = false;
};
```
### debug_agent::Breakpoint::OnHit

This gets called whenever a ProcessBreakpoint is hit (Read below on
ProcessBreakpoint).

The actual handling is very simple: Update the hit count and communicate
whether this is a one shot breakpoint (and thus should be deleted).







