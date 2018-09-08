# HW Breakpoints

Apparently, linux provided a new API for setting up HW breakpoints.
These are called

register_user_hw_breakpoint [ include/linux/hw_breakpoint.h ]

Within [ kernel/events/hw_breakpoint.c ] There is a rundown of the calls

register_user_hw_breakpoint                                                         [ kernel/events/hw_breakpoint.c ]
  perf_event_create_kernel_counter                                                  [ kernel/events/core.c ]
    // Does some tracking about what CPU it's using and what event his is
    perf_install_in_context                                                         [ kernel/events/core.c ]


## HW Breakpoint installation

Most of the code is in [ kernel/events/hw_breakpoint.c ]:

hw_breakpoint_add
  arch_install_hw_breakpoint
    [CASE x86]
    // Searches for a free HW breakpoint
    // and if free, installs the debug register.
    // Interestingly enough, this tracking is per CPU and doesn't seem to have
    // much concepts of threads.



