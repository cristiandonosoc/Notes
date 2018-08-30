# ARM64 HW Debug registers

ARMv8-A holds from 2-16 hw breakpoints and watchpoints.

## HW Breakpoints

Each HW Breakpoint consists of two register, a value and a control one.

### DBGBCR\<n\>\_EL1, Debug Breakpoint Control Register, n=0..15

This controls the way a breakpoint behaves. If an implementation provides less
HW registers, the upper ones are not defined.

Format: Number are in bits
0 [1]:   E ->  Whether the HW breakpoint is enabled.
                0 = disabled
                1 = enabled

1-2 [2]: PMC -> Priviled Mode Control.
                 Determines the exception level at which the breakpoint event is
                 generated.

3-4 [2]: RES0 -> RESERVED

5-8 [4]: BAS -> Byte Address Select.




