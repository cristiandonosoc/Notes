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

Actual architecture implementations reside in `source/Plugins/Process/Utility/*`.
They have a lot of similar classes for <OS>/<ARCH> combinations.
They rely on inheritance interfaces to abstract the arch specifics.

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
