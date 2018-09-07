# Frame Fingerprints

Are a way to refer to a stack frame without keeping the Frame Pointer around
(rbp in x64). They have a consistent address that is valid during the code in
a function.

Specifically, it's a way to compare stack frame across pause/resumes of the same
thread. Frame pointers themselves are owned by the Thread and will be destroyed
when the thread is resumed. (TODO: WHAT DOES THIS MEAN?).

By saving this FrameFingerprint, code can compare whether a future stop is the
same of a subframe of the previous one.

## Stack frames

When stack pointers are not omitted, a typical prologue of a function looks like
this:

```
CallerFunc(...)
  ...
  call CalledFunc       ; This will push the return address into the stack.

... <UNRELATED CALL>


CalledFunc(...)
  push rbp              ; Save the previous stack pointer into the stack.
  move rbp, rsp         ; The base pointer now tracks the top of the stack when
                        ; this function is starting.
  ...

  ; This returns the stack pointer to  where it started, so it can pop the
  ; return address with the ret instruction.
  add rsp, <FUNCTION STACK USAGE OFFSET>
  ret       ; Pops the address that was pushed in by call.


```


