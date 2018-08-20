# ZXDB HW breakpoint proposal

## What would have to change on AgentSide?

Need a way to differentiate between software and hardware
-> Can use BreakpointSettings for that, expose through the Breakpoint interface.

In zircon, the following structs will be implemented:


#if defined(__x86_64__)
typedef struct zx_thread_state_debug_regs {
{
  uint64_t dr[4];
  // DR5 and DR6 are not used.
  uint64_t dr5_reserved;
  uint64_t dr6_reserved;
  uint64_t status;
  uint64_t control;
} zx_thread_state_debug_regs_t;

#elif defined(__aarch64__)

// ARM can hold

typedef struct zx_thread_state_debug_regs {
{
  struct {
    uint64_t dbgbvr
    uint32_t dbgbcr
  } hw__bp[16];
  struct {
    uint64_t dbgwvr
    uint32_t dbgwcr
  } hw__wp[16];
} zx_thread_state_debug_regs_t;

#endif



struct user_hwdebug_state {
	__u32		dbg_info;
	__u32		pad;
	struct {
		__u64	addr;
		__u32	ctrl;
		__u32	pad;
	}		dbg_regs[16];
};




Breakpoint exceptions
MDSCR_EL1.MDE, plus an enable control for each breakpoint, DBGBCR<n>_EL1.E.
Watchpoint exceptions
MDSCR_EL1.MDE, plus an enable control for each watchpoint, DBGWCR<n>_EL1.E

MDSCR_EL1, Monitor Debug System Control Register
32 bit

