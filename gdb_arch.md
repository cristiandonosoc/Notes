GBD supports many archs.
The implementation of the API is in the gdbarch struct, which is a huge grab bag of
function pointers to all the API implemented by the architecute.

Each architecture then implements and registers a global static function that
initializes this struct for each architecture

The function is registered by calling `register_gdbarch_init`, which then calls all
the registered functions at startup.

Normally these archs are defined in a file called `arch`-tdep.h/.c
