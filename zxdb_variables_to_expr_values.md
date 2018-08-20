# ZXDB Variables to expr values

This is converting DWARF variables to expression objects, which are the ones
that can eventually obtain the data out of registers/memory.

## zxdb::SymbolDataProvider (zxdb/client/symbols/symbol_data_provider.h)

Symbol interface to get actual data from the client (and hence the debug_agent).
Provides registers and memory.

## zxdb::FrameSymbolDataProvider (zxdb/client/frame_symbol_data_provider.h)

Implementation of the SymbolDataProvider interface, linked to an actual frame.
The frame has the stack information (SP, IP and access to memory).

Requires to be bound to a frame, and can "disown" it. If not bound to a frame,
all the data request will fail.

## zxdb::SymbolVariableResolver (zxdb/expr/symbol_variable_resolver.h)

Conversion from DWARF symbol to an ExprValue (which can be processed to get an
actual value with the SymbolDataProvider.



