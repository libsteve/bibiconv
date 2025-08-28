//
//  marc8_tables.h
//  bibiconv
//

#ifndef BIBICONV_MARC8_CHARSETS_H
#define BIBICONV_MARC8_CHARSETS_H

#include <bibiconv/codetable.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma mark Basic Latin

/**
 * Lookup table from MARC-8 Basic Latin (ASCII) character set to Unicode.
 *
 * [https://www.loc.gov/marc/specifications/codetables/BasicLatin.html][1]
 *
 * [1]: https://www.loc.gov/marc/specifications/codetables/BasicLatin.html
 */
extern struct bib_iconv_charset const bib_iconv_marc8_basic_latin;

#pragma mark Extended Latin

/**
 * Lookup table from MARC-8 Extended Latin (ANSEL) character set to Unicode.
 *
 * [https://www.loc.gov/marc/specifications/codetables/ExtendedLatin.html][1]
 *
 * [1]: https://www.loc.gov/marc/specifications/codetables/ExtendedLatin.html
 */
extern struct bib_iconv_charset const bib_iconv_marc8_extended_latin;

#pragma mark Greek Symbols

/**
 * Lookup table from MARC-8 Greek Symbols character set to Unicode.
 *
 * [https://www.loc.gov/marc/specifications/codetables/GreekSymbols.html][1]
 *
 * [1]: https://www.loc.gov/marc/specifications/codetables/GreekSymbols.html
 */
extern struct bib_iconv_charset const bib_iconv_marc8_greek_symbols;

#pragma mark Subscript

/**
 * Lookup table from MARC-8 Subscripts character set to Unicode.
 *
 * [https://www.loc.gov/marc/specifications/codetables/Subscripts.html][1]
 *
 * [1]: https://www.loc.gov/marc/specifications/codetables/Subscripts.html
 */
extern struct bib_iconv_charset const bib_iconv_marc8_subscript;

#pragma mark Superscript

/**
 * Lookup table from MARC-8 Superscripts character set to Unicode.
 *
 * [https://www.loc.gov/marc/specifications/codetables/Superscripts.html][1]
 *
 * [1]: https://www.loc.gov/marc/specifications/codetables/Superscripts.html
 */
extern struct bib_iconv_charset const bib_iconv_marc8_superscript;

#pragma mark Basic Hebrew

/**
 * Lookup table from MARC-8 Basic Hebrew character set to Unicode.
 *
 * [https://www.loc.gov/marc/specifications/codetables/BasicHebrew.html][1]
 *
 * [1]: https://www.loc.gov/marc/specifications/codetables/BasicHebrew.html
 */
extern struct bib_iconv_charset const bib_iconv_marc8_basic_hebrew;

#pragma mark Basic Cyrillic

/**
 * Lookup table from MARC-8 Basic Cyrillic character set to Unicode
 *
 * [https://www.loc.gov/marc/specifications/codetables/BasicCyrillic.html][1]
 *
 * [1]: https://www.loc.gov/marc/specifications/codetables/BasicCyrillic.html
 */
extern struct bib_iconv_charset const bib_iconv_marc8_basic_cyrillic;

#pragma mark Extended Cyrillic

/**
 * Lookup table from MARC-8 Extended Cyrillic character set to Unicode.
 *
 * [https://www.loc.gov/marc/specifications/codetables/ExtendedCyrillic.html][1]
 *
 * [1]: https://www.loc.gov/marc/specifications/codetables/ExtendedCyrillic.html
 */
extern struct bib_iconv_charset const bib_iconv_marc8_extended_cyrillic;

#pragma mark Basic Arabic

/**
 * Lookup table from MARC-8 Basic Arabic character set to Unicode.
 *
 * [https://www.loc.gov/marc/specifications/codetables/BasicArabic.html][1]
 *
 * [1]: https://www.loc.gov/marc/specifications/codetables/BasicArabic.html
 */
extern struct bib_iconv_charset const bib_iconv_marc8_basic_arabic;

#pragma mark Extended Arabic

/**
 * Lookup table from MARC-8 Extended Arabic character set to Unicode.
 *
 * [https://www.loc.gov/marc/specifications/codetables/ExtendedArabic.html][1]
 *
 * [1]: https://www.loc.gov/marc/specifications/codetables/ExtendedArabic.html
 */
extern struct bib_iconv_charset const bib_iconv_marc8_extended_arabic;

#pragma mark Basic Greek

/**
 * Lookup table from MARC-8 Basic Greek character set to Unicode.
 *
 * [https://www.loc.gov/marc/specifications/codetables/BasicGreek.html][1]
 *
 * [1]: https://www.loc.gov/marc/specifications/codetables/BasicGreek.html
 */
extern struct bib_iconv_charset const bib_iconv_marc8_basic_greek;

#pragma mark East Asian Characters

/**
 * Lookup table from MARC-8 East Asian Character Codes to Unicode.
 *
 * [https://www.loc.gov/marc/specifications/specchareacc.html][1]
 *
 * [1]: https://www.loc.gov/marc/specifications/specchareacc.html
 */
extern struct bib_iconv_charset const bib_iconv_marc8_eacc;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BIBICONV_MARC8_CHARSETS_H */
