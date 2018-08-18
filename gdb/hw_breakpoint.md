# GDB hw breakpoints looking

How to get into from knowing nothing of hw breakpoints, into reading how it is made in GDB?

1. Download the source
2. Where to start? This is a hw breakpoint, so grep for breakpoint
3. Go to gdb/breakpoint.c
4. See the enum, see what types there are

(Talk about the clang goto tools)

Describe the gdb inheritance tree

## x64 linux implementation

INTERFACE
### can_use_hw_breakpoint

Returns the amount of breakpoints depending on precalculated variables that counts
watchpoint and hw breakpoints available in the architecture.

Where do these variables come from? Grep for them!

HW interface -> gdb/nat/aarch64-linux-hw-point.h



insert_hw_breakpoint
remove_hw_breakpoint

