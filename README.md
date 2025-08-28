# bibiconv

A library for converting MARC-8 encoded text to Unicode.

## Overview

**bibiconv** is a portable C23 library that implements character set conversion
between MARC-8, an encoding used in MARC-21 records, and the modern Unicode
formats UTF-8, UTF-16, and UTF-32. It is designed to closely mirror the behavior 
of the POSIX `iconv` API while providing specialized support for MARC-8,
including its combining characters and multibyte graphic sets.

This library is intended for use in bibliographic software and digital
preservation projects that need to interpret or transcode MARC-21 records into
Unicode safely and correctly.

## Features

- Conversion from **MARC-8** to UTF-8, UTF-16, and UTF-32.
- Conversion between Unicode formats (UTF-8 ↔ UTF-16 ↔ UTF-32).
- Careful handling of combining characters and multibyte code sets
  defined by the [Library of Congress MARC-8 specification][marc8-spec].
- Error reporting via `errno`, consistent with POSIX `iconv`.
- Portable C23 implementation with no external dependencies.
- Thoroughly documented with comments to explain both high-level usage and 
  low-level conversion mechanics.

> **Note:** Conversion from Unicode back to MARC-8 is not yet supported.

[marc8-spec]: https://www.loc.gov/marc/specifications/speccharintro.html

## Usage

The API is modeled after POSIX `iconv`, using a conversion descriptor to manage 
state across multiple calls.

### Opening a descriptor

```c
#include <bibiconv/bibiconv.h>

bib_iconv_t cd = bib_iconv_open("UTF-8", "MARC-8");
if (cd == (bib_iconv_t)-1) {
    perror("bib_iconv_open");
    return;
}
```

Valid encodings include `"MARC-8"`, `"UTF-8"`, `"UTF-16"`, `"UTF-32"`, and
common variations like lowercase or missing dashes, such as `"utf8"` and
`"ucs2"`.

### Converting text

```c
char const source[] = "Caf\xE2\x65"; // "Café" in MARC-8
char *src = (char *)source;
size_t srcleft = strlen(source);

char destination[256];
char *dst = destination;
size_t dstleft = sizeof(destination);

// Perform conversion
size_t result = bib_iconv(cd, &src, &srcleft, &dst, &dstleft);
if (result == (size_t)-1) {
    perror("bib_iconv");
    bib_iconv_close(cd);
    return;
}
```

### Flushing state

Like POSIX `iconv`, the descriptor may buffer pending characters. Always flush
before closing:

```c
result = bib_iconv(cd, nullptr, nullptr, &dst, &dstleft);
if (result == (size_t)-1) {
    perror("bib_iconv flush");
}
```

### Closing the descriptor

```c
bib_iconv_close(cd);
*dst = '\0';
printf("Converted string: %s\n", destination);
```

## Error Handling

When `bib_iconv` fails, it returns `(size_t)-1` and sets `errno` to one of the
following values:

- `EINVAL`: Invalid parameters or unsupported encoding.
- `E2BIG`: Not enough space in the destination buffer.
- `EILSEQ`: Invalid or unrecognized sequence in the source text.
- `ENOMEM`: Out of memory.

The library updates the source and destination pointers after each call so you
can resume conversion after handling errors.

## Supported MARC-8 Character Sets

bibiconv includes tables for all official MARC-8 graphic sets, including:

- Basic Latin
- Extended Latin
- Greek, Hebrew, Cyrillic
- Arabic (basic and extended)
- EACC (East Asian Character Code)
- Superscripts, subscripts, and symbols

These are implemented using trie tables and lookup maps to efficiently handle
both single-byte and multibyte sequences.

## Standards and References

- [ECMA-35 Character Code Structure and Extension Techniques][ecma-35]
  - Equivalent to ISO/IEC 2022 Information technology — Character code structure 
    and extension techniques.
- [ECMA-43 8-Bit Coded Character Set Structure and Rules][ecma-43]
  - Equivalent to ISO 4873 Information technology — ISO 8-bit code for 
    information interchange — Structure and rules for implementation.
- [MARC 21 Specifications for Record Structure, Character Sets, and Exchange 
  Media][marc]
- [Unicode Standard][unicode]

[ecma-35]:
  https://ecma-international.org/publications-and-standards/standards/ecma-35/
[ecma-43]:
  https://ecma-international.org/publications-and-standards/standards/ecma-43/
[marc]:
  https://www.loc.gov/marc/specifications/
[unicode]:
  https://www.unicode.org/

## License

bibiconv is released under the BSD 3-Clause License. See [LICENSE](LICENSE) for
details.

