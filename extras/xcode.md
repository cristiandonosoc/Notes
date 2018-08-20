I've been working on a debugger, and that gives me the perspective about other debuggers.

With the exception of Visual Studio, I avoid any other IDE debugger like the plague.
The problem is not their debugging capabilities per se, but the amount of bureacracy needed to start
debugging.

Keep in mind that I'm not refering to building a binary with the tool. If you are using that IDE
to build, what I'm saying is a little less valid. The usecase I'm referring to is debugging an already
compiled binary with the debug symbols in, with the source code in the correct place. Basically, the
*expected* usecase for a debugger.

But almost without exception, they require to create concepts absolutely foreign do the developer in
order to start debugging. They will require you to create a project, with a lot of setup information
you don't need, as the building was doing elsewhere. Sometimes there is a (very hidden) "empty project"
option that enables you to not the minimum amount of setup. Then you have to find a way of importing
the binary into this project. Sometimes iws just importing the file (xcode), but sometimes is appallingly
hard. I just couldn't find a way to do it in CLion, which required a CMake configuration.

Kepp in mind all this information is _not needed at all_. All the information is already in the binary,
the project settings are not needed at all. Sometimes you need to insert env variables, which I can accept
some UI to use. Though every debugger makes it *incredibly* hard to find. In their effort to make things
easier for the developer, they make it very hard for people that stray even a little bit out of the path.

Tools should just work. Perior. They should only ask for more information if needed. If I have a binary,
I should be able to open and start debugging. No projects, no make, no import config, no nothing.
I do not want to use your IDE setup, I just want to debug. GDB does this out of the box: Just open the binary
and start by pressing r and enter. That's it. And this is a program started in the 70s. 40 years later and
simply starting is harder than ever.

And there is even more amazingly hard stuff. In Xcode, when you already have the deubgging running, in order
to see the disassembly, you need to "edit your scheme for running", whatever that is. The assembly is there.
The debugger is *running* the binary, and if it's able to put breakpoints, it's definitively to match source
and instructions. Why do I need to setup a "Scheme"? What is a scheme and *why* do I need to know about this?
Why push concepts on to the user. This is bad user experience. The only reason I can think of for this are either
misguided engineering, where nothing in your IDE works without these overarching concepts, even when they're not
needed. The other is to try a look in, which is even worse.

If you're making a tool, please make it as straighforward for the user to work. Don't force concepts on them
unless really necesary. Definitively don't create those concepts in the background. They *will* come around to
bite the user. I shouldn't need to write this, but *make things just work*!.


PD: Xcode... chaging the ASM syntax should be a visible button. I couldn't find how to do it? Is it even possible?
