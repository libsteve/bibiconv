# ``static_assert``

Evaluates a constant expression at compile time to assert that it is true.

The `_Static_assert` keyword was introduced to C in the C11 standard as a way
to check invariants at compile time. A `static_assert` macro was added to the
`<assert.h>` header as an alternative to using the keyword. This is a common
practice when introducing new features to C to avoid source-breaking changes.

The `static_assert` keyword was introduced to C in the C23 standard, deprecating
the old `_Static_assert` keyword.

``static_assert`` takes two parameters: the constant expression to evaluate at
compile time, and a message string to print if the expression evaluates to `0`.

C23 added the ability to omit the message string, but for compatibility with
older standards, it's best to always include a message when using this macro.

## Example

```c
constexpr int number = 42;
static_assert(number % 2 == 0, "'number' must be even.");
```
