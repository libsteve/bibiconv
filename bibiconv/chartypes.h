//
//  chartypes.h
//  bibiconv
//

#ifndef BIBICONV_CHARTYPES_H
#define BIBICONV_CHARTYPES_H

#include <stdint.h>

#if __STDC_VERSION__ >= 201710L && !defined(__cplusplus) \
    && (!defined(__has_include) || __has_include(<uchar.h>)) \
    && (defined(__has_include) || !defined(__APPLE__))
#include <uchar.h>
#if !defined(__STDC_VERSION_UCHAR_H__) || __STDC_VERSION_UCHAR_H__ < 202311L
/**
 * A UTF-8 character code unit.
 *
 * A "code unit" is a single atom of information used in a character encoding to
 * represent all or part of a code point.
 *
 * A "code point" is a value that identifies a single character in a coded
 * character set. In an 8-bit character encoding, a code point is represented
 * as one or more 8-bit code units.
 *
 * UTF-8 is an 8-bit character encoding where a single code point is composed of
 * one, two, three, or four 8-bit code units.
 */
typedef unsigned char char8_t;
#endif /* __STDC_VERSION_UCHAR_H__ < 202311L */
#else
#if !defined(__cplusplus) || !defined(__cpp_char8_t)
/**
 * A UTF-8 character code unit.
 *
 * A "code unit" is a single atom of information used in a character encoding to
 * represent all or part of a code point.
 *
 * A "code point" is a value that identifies a single character in a coded
 * character set. In an 8-bit character encoding, a code point is represented
 * as one or more 8-bit code units.
 *
 * UTF-8 is an 8-bit character encoding where a single code point is composed of
 * one, two, three, or four 8-bit code units.
 */
typedef unsigned char char8_t;
#endif /* !defined(__cplusplus) || !defined(__cpp_char8_t) */
#if !defined(__cplusplus) || !defined(__cpp_unicode_characters)
/** 
 * A UTF-16 character code unit.
 *
 * A "code unit" is a single atom of information used in a character encoding to
 * represent all or part of a code point.
 *
 * A "code point" is a value that identifies a single character in a coded
 * character set. In an 16-bit character encoding, a code point is represented
 * as one or more 16-bit code units.
 *
 * UTF-16 is a 16-bit character encoding where a single code point is composed
 * of one or two 16-bit code units.
 */
typedef uint_least16_t char16_t;
/** 
 * A UTF-32 character code point.
 *
 * A "code unit" is a single atom of information used in a character encoding to
 * represent all or part of a code point.
 *
 * A "code point" is a value that identifies a single character in a coded
 * character set. In an 32-bit character encoding, a code point is represented
 * as one or more 32-bit code units.
 *
 * UTF-32 is 32-bit character encoding where a single code point is equivalent
 * to a single 32-bit code unit.
 */
typedef uint_least32_t char32_t;
#endif /* !defined(__cplusplus) || !defined(__cpp_unicode_characters) */
#endif /* __STDC_VERSION__ >= 201710L */

/**
 * A code unit for an 8-bit coded character set.
 *
 * A "code unit" is a single atom of information used in a character encoding to
 * represent all or part of a code point.
 *
 * A "code point" is a value that identifies a single character in a coded
 * character set. In an 8-bit character encoding, a code point is represented
 * as one or more 8-bit code units.
 *
 * UTF-8 is an 8-bit character encoding where a single code point is composed of
 * one, two, three, or four 8-bit code units.
 *
 * MARC-8 is an 8-bit character encoding that specifies both single-byte and
 * multi-byte character sets. Code points for single-byte character sets are
 * equivalent to their code units. MARC-8 only specifies one multi-byte set,
 * the EACC character set, which uses exactly three code units to represent a
 * single code point.
 */
typedef unsigned char codeunit8_t;

/**
 * A code unit for a 16-bit coded character set.
 *
 * A "code unit" is a single atom of information used in a character encoding to
 * represent all or part of a code point.
 *
 * A "code point" is a value that identifies a single character in a coded
 * character set. In an 16-bit character encoding, a code point is represented
 * as one or more 16-bit code units.
 *
 * UTF-16 is a 16-bit character encoding where a single code point is composed
 * of one or two 16-bit code units.
 *
 * UCS-2 is a 16-bit character encoding where a single code point is composed of
 * a single 16-bit code unit. However, this means that it can only represent
 * Unicode code points within the Basic Multilingual Plane (BMP).
 */
typedef uint_least16_t codeunit16_t;

/**
 * A code unit for a 32-bit coded character set.
 *
 * A "code unit" is a single atom of information used in a character encoding to
 * represent all or part of a code point.
 *
 * A "code point" is a value that identifies a single character in a coded
 * character set. In an 32-bit character encoding, a code point is represented
 * as one or more 32-bit code units.
 *
 * Both UTF-32 and UCS-4 are 32-bit coded character sets where a single code
 * point is composed of a single 32-bit code unit.
 */
typedef uint_least32_t codeunit32_t;

/**
 * A UCS-2 character code point.
 *
 * A "code point" is a value that represents a single character in a coded
 * character set. A UCS-2 code point is a single 16-bit code unit that can
 * represent only Unicode characters within the Basic Multilingual Plane (BMP).
 */
typedef codeunit16_t ucs2_t;

/**
 * A UCS-4 character code point.
 *
 * A "code point" is a value that represents a single character in a coded
 * character set. A UCS-4 code point is a single 32-bit code unit that can
 * represent any Unicode character.
 */
typedef codeunit32_t ucs4_t;

/** A MARC-8 character code unit. */
typedef unsigned char marc8_t;

/** An invalid UCS-4 code point. */
#define UCS2_NONCHAR ((ucs2_t)0xFFFF)
/** An invalid UCS-4 code point. */
#define UCS4_NONCHAR ((ucs4_t)0xFFFF)

/** A skippable UCS-2 code point. */
#define UCS2_IGNORED ((ucs2_t)0xFFFE)
/** A skippable UCS-4 code point. */
#define UCS4_IGNORED ((ucs4_t)0xFFFE)

/** The replacement for unknown, unrecognized, or unrepresentable characters. */
#define UCS2_REPLACE ((ucs2_t)0xFFFD)
/** The replacement for unknown, unrecognized, or unrepresentable characters. */
#define UCS4_REPLACE ((ucs4_t)0xFFFD)

#endif /* BIBICONV_CHARTYPES_H */
