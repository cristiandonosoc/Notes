# LLDB Notes

They follow a similar approach as LLVM. They *love* their *huge* interfaces.
1k-2k header files that only define functions are common. This is theoretically
correct, in this sense that the code can be composed as your interface is very
flexible. But it's very hard to understand what an interface does if you need
willpower at this point to grok it. Interfaces should be simple, clean and small!

If you need 70+ functions from an object, I feel something is running amock.

## Breakpoint (Breakpoint/Breakpoint.h)

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

## Target (Target/Target.h)

A target seems to represent the debugged process.

Adding a breakpoint is a little dance:

Target::CreateBreakpoint                      [ Target/Target.cpp ]
  Breakpoint <ctor>                           [ Breakpoint/Breakpoint.cpp ]
  BreakpointResolver->SetBreakpoint           [ Breakpoint/BreakpointResolver.cpp ]
  Target::AddBreakpoint                       [ Target/Target.cpp ]
    BreakpointResolver->ResolveBreakpoint     [ Breakpoint/BreakpointResolver.cpp ]
      SearchFilter->Search                    [ Core/SearchFilter.cpp ]
        // This will call the resolver callback with the correct locations.

## Architecture (Core/Architecture.h)

Permits some specific functions to override stop information or skip preamble
of functions.

## ArchSpec (Utility/ArchSpec.h)

Architecture, CPU, model, OS, byte order, etc.
A grab bag for all the information needed to identify a particular arch.




