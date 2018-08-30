# Expr

Expressions defines to the module that can parse C-ish expressions on the console
and then being able to evaluate it to data.

The code lives in [ zxdb/expr ]

## ExprToken [ zxdb/expr/expr_token.h ]

Basic token type that suppots C-ish operators (., \*, \&, ->, [. ], etc.).

## ExprParser [ zxdb/expr/expr_parser.h ]

The expression parser will tokenize and then create an AST with the expression.
The parser is a Pratt parser.

TODO: Explain Pratt parsing mechanism.

The parsing will return the root node of the AST.

## ExprValue [ zxdb/expr/expr_value.h ]

Holds the value for a particular expression. Can be both program variables
(eg. local variable on the stack) or a calculated expression by the debugger.

A value holds:
- A type
- A value source (basically a wrapper over a memory address).
- A vector of byte data.

Has a lot of constructors for wrapping int types, but can also hold the data
for arbitraty types.

## ExprNode [ zxdb/expr/expr_node.h ]

Interface that represents one node within the expression AST.
Each node has to inherit (Array access, address of (&), etc).

Each particular specialized node has the subexpressions it needs to evaluate
the values. For example, `ArrayAccessExprNode` has as members a left expressions
and a inner expressions, which corresponts to the pattern `<left>[<inner>]`.

This boils down to "leaf" nodes, such as `IdentifierExprNode` or `IntegerExprNode`,
which holds `ExprToken`as members.

### MemberAccessExprNode

This is an interesting example as it needs async data. Implements both `.` and
`->` operators for data member access. The format is `<left><accessor><member>`.

The evaluation is serial in fashion, chained by callbacks.

First evaluate the left expression (call eval on the ExprNode).
  [IF . ACCESS] ResolveMember
  [ELSE] ResolveMemberByPointer

#### ResolveMember

Resolve member can be passed either a member name or the DataMember itself.
DataMember is a Value (which is a symbol), which also has information about
the byte offset of the member within the script.

If the name is given, it will evaluate all the children upon finding the
one with the member name.

The resolve is rather simple:

Get a reference to the base class and compare the offset and size of the
member type with the data within the holding expression. If the match,
create a new ExprValue with the type and data.
