# ZXDB Message Loops

Message loops are simply a thread loop into which other threads can post tasks,
and the loop will get to them.

It also listens to interesting events (network activity, exceptions, etc.)

## debug_ipc::MessageLoop (lib/debug_ipc/helpers/message_loop.h)

The virtual interface to message loops. has two specialization:
- MessageLoopZircon: Meant to run in the target
- MessageLoopPoll: Meant to run in the client. Very Linux oriented, uses Poll to wait
                   on file descriptors.

MessageLoop are thread dependent, and there should only be one loop per thread:

Each thread has a `thread_local` pointer to a message_loop defined in
lib/debug_ipc/helper/message_loop.cc. It is null by default, but calling Init
on a MessageLoop will bind (set the pointer) it to that thread. Calling Cleanup will
remove that pointer.

From that point forward, the static getter for that thread will return the bound
message loop. In general, you don't want to be playing much binding/unbinding
message loops.

The intended usage is in the "loop" thread to bind it and call Run, which will
call the system dependent RunImpl, which will indefinitively run handling tasks
and events.

The code should get a reference to *that* message loop so other threads can post
tasks to that message loop. Otherwise they will try to post tasks to their thread
local message loop, which is either not bound or another message loop. And generally
you don't want to have two concurrent message loops.

Posting a task (debug_ipc::MessageLoop::PostTask) is thread safe.

## debug_ipc::MessageLoopPoll (lib/debug_ipc/helpers/message_loop_poll.h)

Specialization to run on Linux like systems where poll is available.

It has a mapipng from fd to a WatchInfo. Clients of the message loop will suscripe
to watches, and they will get called when an event on that fd occurs.


## debug_ipc::MessageLoopZircon (lib/debug_ipc/helpers/message_loop_zircon.h)

TODO(donosoc): Make this writeup
