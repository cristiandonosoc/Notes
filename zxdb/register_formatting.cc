

// Status flags
constexpr int RFLAGS_CARRY_FLAG_SHIFT = 0;
constexpr int RFLAGS_PARITY_FLAG_SHIFT = 2;
constexpr int RFLAGS_AUX_CARRY_FLAG_SHIFT = 4;
constexpr int RFLAGS_ZERO_FLAG_SHIFT = 6;
constexpr int RFLAGS_SIGN_FLAG_SHIFT = 7;
constexpr int RFLAGS_TRAP_FLAG_SHIFT = 8;
constexpr int RFLAGS_INTERRUPT_ENABLE_FLAG_SHIFT = 9;
constexpr int RFLAGS_DIRECTION_FLAG_SHIFT = 10;
constexpr int RFLAGS_OVERFLOW_FLAG_SHIFT = 11;

#ifdef ENABLE_X64_SYSTEM_RFLAGS

constexpr int RFLAGS_IO_PRIVILEDGE_LEVEL_SHIFT = 12;
constexpr int RFLAGS_NESTED_TASK_SHIFT = 14;
constexpr int RFLAGS_RESUME_FLAG_SHIFT = 16;
constexpr int RFLAGS_VIRTUAL_8086_MODE_SHIFT = 17;
constexpr int RFLAGS_ALIGNMENT_CHECK_SHIFT = 18;
constexpr int RFLAGS_VIRTUAL_INTERRUPT_FLAG_SHIFT = 19;
constexpr int RFLAGS_VIRTUAL_INTERRUPT_PENDING_SHIFT = 20;
constexpr int RFLAGS_ID_FLAG_SHIFT = 21;

#endif

#define FLAG_VALUE(value, shift, mask) (uint8_t)((value >> shift) & mask)




  FlagValue(&rows, value, "Carry Flag", RFLAGS_CARRY_FLAG_SHIFT, 0x1);
  FlagValue(&rows, value, "Parity Flag", RFLAGS_PARITY_FLAG_SHIFT, 0x1);
  FlagValue(&rows, value, "Aux Carry Flag", RFLAGS_AUX_CARRY_FLAG_SHIFT, 0x1);
  FlagValue(&rows, value, "Zero Flag", RFLAGS_ZERO_FLAG_SHIFT, 0x1);
  FlagValue(&rows, value, "Sign Flag", RFLAGS_SIGN_FLAG_SHIFT, 0x1);
  FlagValue(&rows, value, "Trap Flag", RFLAGS_TRAP_FLAG_SHIFT, 0x1);
  FlagValue(&rows, value, "Interrupt Enable Flag",
            RFLAGS_INTERRUPT_ENABLE_FLAG_SHIFT, 0x1);
  FlagValue(&rows, value, "Direction Flag", RFLAGS_DIRECTION_FLAG_SHIFT, 0x1);
  FlagValue(&rows, value, "Overflow Flag", RFLAGS_OVERFLOW_FLAG_SHIFT, 0x1);

#ifdef ENABLE_X64_SYSTEM_RFLAGS

  // TODO(donosoc): Do the correct formatting when we enable this.

  FlagValue(&rows, value, "I/O Priviledge Level",
            RFLAGS_IO_PRIVILEDGE_LEVEL_SHIFT, 0x2);
  FlagValue(&rows, value, "Nested Task", RFLAGS_NESTED_TASK_SHIFT, 0x1);
  FlagValue(&rows, value, "Resume Flag", RFLAGS_RESUME_FLAG_SHIFT, 0x1);
  FlagValue(&rows, value, "Virtual 8086 Mode", RFLAGS_VIRTUAL_8086_MODE_SHIFT,
            0x1);
  FlagValue(&rows, value, "Alignment Check", RFLAGS_ALIGNMENT_CHECK_SHIFT, 0x1);
  FlagValue(&rows, value, "Virtual Interrupt Flag",
            RFLAGS_VIRTUAL_INTERRUPT_FLAG_SHIFT, 0x1);
  FlagValue(&rows, value, "Virtual Interrupt Pending",
            RFLAGS_VIRTUAL_INTERRUPT_PENDING_SHIFT, 0x1);
  FlagValue(&rows, value, "ID Flag", RFLAGS_ID_FLAG_SHIFT, 0x1);

#endif


