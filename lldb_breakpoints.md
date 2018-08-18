# LLDB Notes

They follow a similar approach as LLVM. They *love* their *huge* interfaces.
1k-2k header files that only define functions are common. This is theoretically
correct, in this sense that the code can be composed as your interface is very
flexible. But it's very hard to understand what an interface does if you need
willpower at this point to grok it. Interfaces should be simple, clean and small!

If you need 70+ functions from an object, I feel something is running amock.

### Breakpoint (Breakpoint/Breakpoint.h)

An LLDB breakpoint has 4 parts:
- A filter
- A resolver,
- A list of locations (determined by the filter/resolver pair)
- A set of options

The resolver is an heuristic about how to find breakpoint locations.
Provides the functionality that the filter requires to actually find
the locations. The filter will do most of the symbol work, while the
resolver is more concerned with the actual addresses.

A breakpoint has a set of locations. Both can have options, but the owning
breakpoint options trumps the individual locations.

### Target (Target/Target.h)

A target seems to represent the debugged process.

Adding a breakpoint is a little dance:

Target::CreateBreakpoint                      [ Target/Target.cpp ]
  Breakpoint <ctor>                           [ Breakpoint/Breakpoint.cpp ]
  BreakpointResolver->SetBreakpoint           [ Breakpoint/BreakpointResolver.cpp ]
  Target::AddBreakpoint                       [ Target/Target.cpp ]
    BreakpointResolver->ResolveBreakpoint     [ Breakpoint/BreakpointResolver.cpp ]
      SearchFilter->Search                    [ Core/SearchFilter.cpp ]
        // This will call the resolver callback with the correct locations.

### Architecture (Core/Architecture.h)

Permits some specific functions to override stop information or skip preamble
of functions.

### ArchSpec (Utility/ArchSpec.h)

Architecture, CPU, model, OS, byte order, etc.
A grab bag for all the information needed to identify a particular arch.

### Watchpoint (Breakpoint/Watchpoint.h)

Simple grab-bag of configurations.

## Register Implementation in LLDB

Of course, they implement their own ptrace wrapper and wrapper for the Linux
context:

### RegisterSet (lldb-private-types.h)

Simple structure that holds a "register set", which is a logical separation of a
group of registers (eg. General Purpose, Floating Point, etc.).
It's a name and array of integer indices that then have to be interpreted by the
corresponding RegisterContext (those indices don't have inherent meaning by
themselves).

### RegisterInfo (lldb-private-types.h)

Defines the generic context for one register. Holds information about name, bytes,
offsets and some DWARF mappings. It holds enough information for sub-registers
(eg. RAX within EAX).

### RegisterContext (Target/RegisterContext.h)

Main interface to the actual information about the registers of a particular
architecture. Each arch will inherit from this class and the rest of the system will
access values of archs from there.

There are some semantic meaning for some registers in lldb::RegisterKind

In lldb-enumarations.h:
```
enum RegisterKind {
  eRegisterKindEHFrame = 0, // the register numbers seen in eh_frame
  eRegisterKindDWARF,       // the register numbers seen DWARF
  eRegisterKindGeneric, // insn ptr reg, stack ptr reg, etc not specific to any
                        // particular target
  eRegisterKindProcessPlugin, // num used by the process plugin - e.g. by the
                              // remote gdb-protocol stub program
  eRegisterKindLLDB,          // lldb's internal register numbers
  kNumRegisterKinds
};
```

Most common usage will be ther DWARF access. DWARF defines it's own register
numbering scheme which has to be mapped for each architecture.

This class also has an API for PC, SP, Hardware breakpoints, etc.

## Process/Thread architecture

The main interface is Process (Target/Process.h). It's *huge* (the header is over
3200 lines long). This is a big interface for particular processes to implement.

It roughly divides into UNIX/Linux vs Windows:
source/Plugins/Process/gdb-remote/ProcessGDBRemote.h
source/Plugins/Process/Windows/Common/ProcessWindows.h

### Thread (Target/Thread.h)

Generic wrapper over a thread. Holds both the pid and tid.
The interface is fairly straightforward: SP, StackFrame.

Contains something called the "ThreadPlan"

#### ThreadPlan (Target/ThreadPlan.h)

ThreadPlan is actually a pure virtual interface.
Represents the behaviours that a particular thread can have upon resumen (and
possibly other states).
You can push plans into the "Plan Stack". Each plan within the stack
can have many actions and the top one is the current, but it can refer
to higher ones for data.
####

## Server Side -------------------------------------------------------------------------------------

### NativeProcessLinux (Plugins/Linux/NativeProcessLinux.h)

This is a wrapper over the actual linux process, so this (or most probably a
derived type) is the interface that the server uses to deal with the actual
process.

Actually, this class will start the underlying process upon construction.
Eventually, the interface if Ptrace, over which they created a wrapper.

### NativeThreadProtocol (Host/common/NativeThreadProtocol.h)

Represents an overarching interface for dealing with "native" Threads in any target.
This is to be specialized by



### NativeRegisterContext (Plugins/Process/<OS>/*)

Implements the NativeRegisterContext (Host/common/NativeRegisterContext.h),
which seems to be used by the NativeThreadProtocol

This is very similar to the Utility implementations residing in
`source/Plugins/Process/Utility/*`.
They have a lot of similar classes for <OS>/<ARCH> combinations.
They rely on inheritance interfaces to abstract the arch specifics.

The hardware registers are also read through ptrace



The resume process goes something like this

### Process resume workflow

#### GDB Client side

Process::Resume                                 [ Target/Process.h ]
  Process::PrivateResume                        [ Target/Process.h ]
    Process::WillResume <<VIRTUAL>>             [ Target/Process.h ]
      ProcessGDBRemote::WillResume              [ Plugins/Process/gdb-remote/ProcessGDBRemote.h ]
        // Justclears it's thread id statuse
      ThreadList::WillResume                      [ Target/ThreadList.h ]
        ∀ contained thread:
        // Does a lot of bookkepping to see if some thread wants to stop another,
        // deciding which will eventually actually run and if they want to run alone.
        // Seems overly complicated at a glance.
        // In between, calls the important function
        Thread::SetupForResume                    [ Target/Thread.h ]
          // Checks if the thread is over a breakpoint and schedules
          // a StepOver plan if needed.
      // If a thread wants to run alone, don't resume the process (not exactly
      // sure what that means).
      Process::RunPreResumeActions
        // Checks and notifications
      Process::DoResume << VIRTUAL >>
        ProcessGDBRemote::DoResume
          // This function is insane. Creates a string encoded "packet" that encodes
          // everything that it wants to do and send it to the GDB client.
          // This is hard because of the GDB protocol.
          // Then sends it over the wire to the server.
          [ Plugins/Process/gdb-remote/ProcessGDBRemote.h ]
          GDBRemoteCommunicationClient::SetCurrentThreadForRun
            // Sends an "Hc" packet

#### GDB Server side

[ Plugins/Process/gdb-remote/GDBRemoteCommunicationServer.h ]
GDBRemoteCommunicationServer::GetPacketAndSendResponse
  // Extracts the packet. There is a *ton* of packet types, they're all defined
  // in [ Utility/StringExtractorGDBRemote.h ]. And that huge enums are in some
  // cases categories of packets!
  // From the packet type we get a handler, registered through
  // `GDBRemoteCommunicationServer::RegisterPacketHandler`,
  // which most important ones are registered through
  // `RegisterMemberFunctionHandler` in
  // [ Plugins/Process/gdb-remote/GDBRemoteCommunicationsServerCommon.h ], which
  // in turns delegate again to member functions of the class
  // GDBRemoteCommunicationServerLLGS [ same path ].
  // We're interested in the H packets
  GDBRemoteCommunicationServerLLGS::Handle_H
    // Validates the packet and gets the NativeThreadProtocol, which handles the
    // local thread.

    ∀ PreResumeActionCallback -> call
