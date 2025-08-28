# ``bibiconv``

A library for converting MARC-8 encoded text to Unicode.

## Overview

bibiconv is a portable C23 library that implements character set conversion
between the legacy MARC-8 encoding used in MARC-21 records, and the modern 
Unicode encodings UTF-8, UTF-16, and UTF-32. It is designed to closely mirror 
the behavior  of the POSIX `iconv` API while providing specialized support for 
MARC-8.

This library is intended for use in bibliographic software and digital
preservation projects that need to interpret or transcode MARC-21 records into
Unicode safely and correctly.

## Topics

### Conversions

- ``bib_iconv``
- ``bib_iconv_t``
- ``bib_iconv_open``
- ``bib_iconv_close``

### Header Files

- <doc:chartypes.h>
- <doc:codetable.h>
- <doc:marc8_tables.h>
- <doc:cfeatures.h>
- <doc:cstderrno.h>

