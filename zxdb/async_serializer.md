# zxdb::FormatValue [ zxdb/console/format_value.h ]

This class is interesting because it permits to output in a single buffer a lot of
values, some of them can be async. When everything is completed, the final callback will
be outputted.

The class has
- A complete callback (which can be set at any time).
- A vector of variable resolvers.
- A root OutputNode

## zxdb::FormatValue::OutputNode

An output node represents a value to be outputted. Because the output of a struct or
array is inherently a tree, the output is also represented by that, where each OutputNode
has a lot of OutputNode children.

## Appending

Appending values require a SymbolContext, a SymbolDataProvider and
the Value/Variable to output.

- SymbolContext: Holds the information to swap between relative and absolute addresses.
- SymbolDataProvider: Provides memory and register data.

Each appending will add the resolution to a new output buffer and mark that we're
waiting for another resolution. The appending code has access to the index of the
buffer so that's able to write to it.

The resolution is two-fold:

- If variable -> Use SymbolVariableResolver to get the value -> Goto value
- If value -> FormatExprValue

The Basic Append adds a variable to the root node. The BasicAsyncAppend simply
pushes the ObjectNode to the parent and marks that more resolution is needed.

This decrease in pending will be done by OutputKeyComplete.

## FormatExprValue

This function needs a buffer to write to. The append call will pass the correct
call to it. Now it receves an OutputKey, which is a key to find the correct
ObjectNode to write to. This key requires to be interpreted by the
FormatValue class, which forces the callbacks to resolve the weak pointer first.
This is for the case where the FormatValue goes away and the callbacks are
still going around.

### Async Case

The async expr formatting is interesting because it handles arrays and strings.
First it checks if this is a string, basically by checking if this is a pointer
or and array of chars.

#### [IF STRING] FormatString

We check whether the value is a pointer and that is not null (could happen if the
variable is not already set).

There is a setting that defines what the max size of an array is (as of this writing
is 256), so we speculative as for that amount if the array size if not known.

Then it will fetch the data and create a callback that calls the initial callback
once the formatting of the received memory is done.

The formatting is straightforward. Get the len of the of the string (or the max
size of the buffer, doesn't need to be a 0-terminated string). Escape the
correct characters and mark if the string seems to be truncated.

#### [ELSE] FormatArray

Gets min(count, max elements), as the size of the array could not be known in
advance.

Then fetch the memory with a callback that will call the initial callback once
the formatting of the array data is done.

## Async completeness

After each Formatting is done, the code will call, either directly or by callback,
will call DecrementPendingResolution, which will decrease the amount of pending
callbacks and then check if the formatting is done.
If that is the case, call the assigned callback or wait until one if given.
