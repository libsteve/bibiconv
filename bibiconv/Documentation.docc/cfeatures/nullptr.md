# ``nullptr``

A constant representing the "null" pointer of type.

This value was was introduced to C in the C23 standard to make it possible for
`_Generic` macros to distinguish a null pointer constant from other pointer
values.

From C23 on, it's good practice to use `nullptr` instead of `NULL`.
