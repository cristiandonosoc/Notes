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
