# Register sets Linux

Linux keeps track of the registers set (the thread state) in `struct user_regset`.
This struct tracks part of the state of an indivual thread, and does not
represents the actual CPU registers per se.

As per v4.17.12:

struct user_regset {
  // Max amount of registers slots.
  unsigned int n;
  // Size of each register slot. Doesn't necessarily
  // match the actual size of the register it tracks.
  unsigned int size;
  // Memory alignment of each slot. >= size.
  unsigned int align;
  // Difference between natural (local) index and regset index.
  unsigned int bias;
  // Annotation of what the regset is.
  // Main bulk of definitions in include/uapi/linux/elf.h.
  unsigned int core_note_type;

  // Interface -----------------------------------------------------------------

  // Fetch values.
  user_regset_get_fn* get;
  // Store values.
  user_regset_set_fn* set;
  // Whether the regset is active, or NULL.
  user_regset_active_fn* active;
  // Function to write data back to user memory, or NULL.
  user_regset_writeback_fn* writeback;
  // Return the regset actual size, or NULL.
  user_regset_get_size_fn* get_size;
}

## ARM64 register tracking structs

The definitions are in arch/arm64/include/uapi/asm/ptrace.h

Defines three register structs:

### General purpose registers

struct user_pt_regs {
  uint64_t regs[31];
  uint64_t sp;
  uint64_t pc;
  uint64_t pstate;
};

### Floating point/vector registers

struct user_fpsimd_state {
  uint128_t vregs[32];
  uint32_t fpsr;
  uint32_t fpcr;
  uint32_t __reserved[2];
};

### Debug registers

struct user_hwdebug_state {
  uint32_t dbg_info;
  uint32_t pad;

  // Up to 16 debug regs.
  struct {
    uint64_t addr;
    uint32_t ctrl;
    uint32_t pad;
  } dbg_regs[16];
};

## ARM64 regset definition

The definition is in arch/arm64/kernel/ptrace.c:974, which defines a user_regset
array, indexed by values in the aarch64_regset enum (same file).

### HW breakpoint regset definition

[REGSET_HW_BREAK] = {
  .core_note_type = NT_ARM_HW_BREAK,                    // ARM hw registers.
  .n = sizeof(struct user_hw_debug_state) / size(u32),  // Length of the user_hw_debug_state array.
  .size = sizeof(u32),
  .align = sizeof(u32),
  .get = hw_break_get,
  .set = hw_break_set,
}
