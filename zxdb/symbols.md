# Symbols

## Symbol [

Symbols are implemented as an simple RTTT based on Symbol [ symbols/symbol.h ].

There Symbol defines a virtual function for "casting" to every type that's handled.
By defaul, Symbol will define these functions return null, as the Symbol has no
inherent type. Types are to inherit from Symbol and override the respective
function.

A Symbol also can have a parent symbol, this is used for classes.
This symbols interface is agnostic to what format the actual symbols are obtained
from. This information is extracted with the use of SymbolFactories (described
further down).

## LazySymbol [ zxdb/client/symbols/lazy_symbol.h ]

This class holds the information to construct the type, but will only do it on
demand. It will also cache the result once calculated. Normally, all symbol
references will be LazySymbols.

In order to create the symbol, it uses a SymbolFactory. LazySymbol basically
holds all the information needed for the SymbolFactory to work (as of today,
a pointer to the data and the offset within that).

## SymbolFactory

Converts from a LazySymbol to a real Symbol. The class holds a weak pointer
to the underlying module symbols. Those symbols could disappear when the module
gets unloaded, but the factory will still be around.

Each factory can have a specialization depending on what format of symbols

### DwarfSymbolFactory

Specialization for extracting DWARF symbols. It uses LLVM for making the actual
DWARF parsing, which the factory will then interpret and transform into zxdb
internal symbol representation.

In order to be able to decode a symbol, it requires a pointer to the data, which
in this case is the compile unit base pointer and the offset to where the particular
DIE is. Where this pointers are obtained from is beyond the scope of the symbol
decoding.

```
ASIDE:
DIE stands for Debug Information Entry, which is the basic block DWARF uses to
describe a program. Each DIE has a type and describes a particular concept (eg. a
function or a member). These DIEs are chained together to form a tree.
```

#### DWARF Decoding

Once the factory has loaded from LLVM the correct DIE, it will look at the type
and decode create the correct symbol from it.

DwarfSymbolFactory::CreateSymbol
  DwarfSymbolFactory::DecodeSymbol
    [SWITCH according to the DIE tag]
      [Specific tag decoding]

Each specific tag will require a DwarfDecoder that provides an API for obtained
that data from the LLVM parsed data.

#### DwarfDieDecoder

TODO: Do DwarfDieDecoding

Examples:

*Base Type*

A BaseType is the set of building blocks that a program can then create further
types from. In C, this would be types such as `char`, `int`, etc.

In order to decode a base type

