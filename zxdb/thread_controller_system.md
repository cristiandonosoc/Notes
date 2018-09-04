# Thread Controller System

There are internal breakpoints. They are not reported to the user, but rather
used to implement other kind of functionality.

There are now ThreadControllers, which define how a thread should behave after
an exception.

ThreadControllers are appended into a Thread. Whenever an exception occurs,
the thread will ask all the controllers whether it should stop. A Controller
will tell continue if the exception does not apply to him
If all the controllers tell continue, the thread will continue.

External breakpoints will always stop. Only external breakpoints will be
documented.

The installation of a Controller is done through `ThreadController::InitWithThread`
It is called with a thread pointer (this is a not owning) and the controller will
call a callback when the instalation is complete.

The idea is that a Thread gets called into Thread::ContinueWith, which basically
says "I want you to restart once this particular controller is installed". That
controller could set up a bunch of breakpoints or some other configuration.

Each controller, when asked what continue strategy to use, returns a
`ThreadController::ContinueOP` to determine what the agent should do (continue,
step in range, step instruction, etc).

## UntilThreadController

This controller is meant to run until a certain location.
In that sense, it owns an internal breakpoint. On installation it will set it.

When the breakpoint is hit, through the controller check mechanism in thread,
it will called be here. This controller will check if it's the same breakpoint
and do a comparison about the frame via the base pointer.

When setting the until controller, you can set up a "frame" where you want this until
to stop. If the frame is lower than that, it means it's further down call to this.

## FinishThreadController

The FinishThreadController has a UntilThreadController as a member.
On install time, it will check if the thread has all the frames. If not, it will
ask for then and then continue.

Then it puts a pointer on the address of the next pointer and let UntilController
take it from there.

## StepInRangeController

This is a controller that will single step while the CPU is excepted within a
particular range.


