# Threads

# thread_t [ kernel/include/kernel/thread.h ]

Some interesting facts about threads

- zx_duration_t runtime_ns:
  Measures the time the thread has been running. But it only counds the time
  up to the last scheduler run. If the thread is running, the counter won't
  measure that until the scheduler run.

- struct arch_thread arch:
  It's (not a pointer interestingly) member that holds all the architecture
  dependent information for the thread.

  This is defined through [ kernel/include/arch/thread.h ]
  which includes [ kernel/arch/<ARCH>/include/arch/arch_thread.h ] where
  the structure is defined.

# Context Switch

There are several reasons for context switching: yields, timers,
Eventually they call

## void sched_resched_internal(void) [ kernel/include/kernel/sched.h ]
This function takes the current thread and switches it with the next one in line
for that CPU.

Then accounts for the time the old thread has been running.
There is the concept of the idle thread.
Then checks for realtime

Makes accounting for realtime threads.

Then calls final_context_switch

## final_context_switch [ kernel/kernel/sched.cpp ]

- Sets the current thread for the CPU

In x86, this writes into the GS register.
In arm64, this writes into the TPIDR_EL1 register.
Both write the thread pointer location.

and calls the arch dependent context switch.

## arch_context_switch

### x86

arch_context_switch
  x86_extended_register_context_switch
    // Calls xsaves, that uses the CPU to save all the state into memory



