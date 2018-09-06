# x64 Stack ABI



Function calling uses the stack for functioning.
For that is uses a convention about what the different registers mean in terms
of the stack and function calling.
Note that the stack in x64 grows "downward". This means that it goes from high
address to low address.

For x64, the convention goes a little bit like this:

- rax: Function return value, if it exists and is no more that 64-bit long.
- rsp: Stack pointer. Points to the topmost element on the stack.
- rbp: Represents the base pointer, where the frame of the current funtion starts.

The first 6 arguments of a function are passed through registers. The 7th and
onwards are passed through the stack. The registers are used in the following
order:

1. rdi
2. rsi
3. rdx
4. rcx
5. r8
6. r9

In 32-bit x86, the register ebp (now rbp) was used to keep track of the base
of the current stack frame. This is called the *base pointer*.
The called function would save the previous base pointer, which permits to do
stack unwinding.

In 64-bit archs, this is not used so much:
TODO: DESCRIBE WHY THIS IS NOT USED.



## Prologue and Epilogue

Each function has setup and teardown. These are called prologue and epilogue
respectively.


## References

https://eli.thegreenplace.net/2011/02/04/where-the-top-of-the-stack-is-on-x86
https://eli.thegreenplace.net/2011/09/06/stack-frame-layout-on-x86-64/
https://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf
