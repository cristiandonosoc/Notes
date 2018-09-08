# Zircon Register Scrapping

Register scrapping is rather straightforward.
From the point of view of the debugger, the register values are obtained
from the following syscalls:

- zx_thread_read_state
- zx_thread_write_state

Those functions are checked wrappers over ThreadDispatcher

## Call stack

zx_thread_read_state                [ ABIGEN: kernel/syscalls/task.cpp ]
  ThreadDispatcher::ReadState       [ kernel/object/thread_dispatcher.cpp ]
    // Cased over the Register set, example: general registers
    arch_get_general_regs   [ kernel/include/arch/debugger.h ]
      // This hold the actual implementation depending on the platform.
      // Will copy the information from the internal structs that hold the
      // register information into the structs defined for the debug purposes,
      // defined in: system/public/zircon/syscalls/debug.h
      [IMPLEMENTATIONS:
        x86_64: kernel/arch/x86/debugger.cpp
        arm64: kernel/arch/arm64/debugger.cpp
      ]

## HW registers

Reading a HW breakpoint is handled like anything else:

READ:   asm("mov %%dbX, %0", :"=r" (out_val));
WRITE:  asm("mov %0, %%dbX", :"r" (in_val));

