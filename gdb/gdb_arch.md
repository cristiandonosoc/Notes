GBD supports many archs.
The implementation of the API is in the gdbarch struct, which is a huge grab bag of
function pointers to all the API implemented by the architecute.

Each architecture then implements and registers a global static function that
initializes this struct for each architecture

The function is registered by calling `register_gdbarch_init`, which then calls all
the registered functions at startup.

Normally these archs are defined in a file called `arch`-tdep.h/.c

## Breakpoints

Breakpoints are tracked in a two-tiered fashion within gdb. The first tracking is
high level: Which addresses it tracks the breakpoint type, location mapping,
commands to run etc:

```
struct breakpoint
{
  virtual ~breakpoint ();

  /* Methods associated with this breakpoint.  */
  const breakpoint_ops *ops = NULL;

  breakpoint *next = NULL;
  /* Type of breakpoint.  */
  bptype type = bp_none;
  /* Zero means disabled; remember the info but don't break here.  */
  enum enable_state enable_state = bp_enabled;
  /* What to do with this breakpoint after we hit it.  */
  bpdisp disposition = disp_del;
  /* Number assigned to distinguish breakpoints.  */
  int number = 0;

  /* Location(s) associated with this high-level breakpoint.  */
  bp_location *loc = NULL;

  /* True means a silent breakpoint (don't print frame info if we stop
     here).  */
  bool silent = false;
  /* True means display ADDR_STRING to the user verbatim.  */
  bool display_canonical = false;
  /* Number of stops at this breakpoint that should be continued
     automatically before really stopping.  */
  int ignore_count = 0;

  /* Number of stops at this breakpoint before it will be
     disabled.  */
  int enable_count = 0;

  /* Chain of command lines to execute when this breakpoint is
     hit.  */
  counted_command_line commands;
  /* Stack depth (address of frame).  If nonzero, break only if fp
     equals this.  */
  struct frame_id frame_id = null_frame_id;

  /* The program space used to set the breakpoint.  This is only set
     for breakpoints which are specific to a program space; for
     non-thread-specific ordinary breakpoints this is NULL.  */
  program_space *pspace = NULL;

  /* Location we used to set the breakpoint.  */
  event_location_up location;

  /* The filter that should be passed to decode_line_full when
     re-setting this breakpoint.  This may be NULL, but otherwise is
     allocated with xmalloc.  */
  char *filter = NULL;

  /* For a ranged breakpoint, the location we used to find the end of
     the range.  */
  event_location_up location_range_end;

  /* Architecture we used to set the breakpoint.  */
  struct gdbarch *gdbarch = NULL;
  /* Language we used to set the breakpoint.  */
  enum language language = language_unknown;
  /* Input radix we used to set the breakpoint.  */
  int input_radix = 0;
  /* String form of the breakpoint condition (malloc'd), or NULL if
     there is no condition.  */
  char *cond_string = NULL;

  /* String form of extra parameters, or NULL if there are none.
     Malloc'd.  */
  char *extra_string = NULL;

  /* Holds the address of the related watchpoint_scope breakpoint when
     using watchpoints on local variables (might the concept of a
     related breakpoint be useful elsewhere, if not just call it the
     watchpoint_scope breakpoint or something like that.  FIXME).  */
  breakpoint *related_breakpoint = NULL;

  /* Thread number for thread-specific breakpoint, or -1 if don't
     care.  */
  int thread = -1;

  /* Ada task number for task-specific breakpoint, or 0 if don't
     care.  */
  int task = 0;

  /* Count of the number of times this breakpoint was taken, dumped
     with the info, but not used for anything else.  Useful for seeing
     how many times you hit a break prior to the program aborting, so
     you can back up to just before the abort.  */
  int hit_count = 0;

  /* Is breakpoint's condition not yet parsed because we found no
     location initially so had no context to parse the condition
     in.  */
  int condition_not_parsed = 0;

  /* With a Python scripting enabled GDB, store a reference to the
     Python object that has been associated with this breakpoint.
     This is always NULL for a GDB that is not script enabled.  It can
     sometimes be NULL for enabled GDBs as not all breakpoint types
     are tracked by the scripting language API.  */
  gdbpy_breakpoint_object *py_bp_object = NULL;

  /* Same as py_bp_object, but for Scheme.  */
  gdbscm_breakpoint_object *scm_bp_object = NULL;
};
```

A breakpoint can consist of many locations. These are represented by the struct
`bp_location`. This is the specific information about the installation.

### The triggering process


watchpoints_triggered   [gdb/breakpoint.c]


Breakpoints will normally trigger a signal which will be relayed into gdb,
in `handle_signal_stop` [gdb/infrun.c]

The whole infrun.c file has the info about how gdb handles an event.

