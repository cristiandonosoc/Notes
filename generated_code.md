# Generated code rant

Sometimes generating code is simply the easiest way to go.
But really consider if you need it and dont go if it's a go to.

Why?

Readability.
Someone looking at your source will have a hard time understanding if
he is not familiar with the other system
If he uses inspection tools (clang), many symbols simply are not there,
and are not even greppable

Want to see what this struct is like? Nope, it's generated.
I understand some kind of architecture dependent, but there's value
in your data structures bein able to hold the complexity in the problem
by themselves.

The code will be *much* more readable.

And exception can be RPC, because making the serializing/deserializing
code is a pain in the ass and the portability makes it very hard.
So tools such as protobuf and the such makes sense.

But don't do it for other stuff unless you *really* need to.
