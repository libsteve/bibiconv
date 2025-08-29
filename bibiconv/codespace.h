//
//  codespace.h
//  bibiconv
//

#ifndef BIBICONV_CODESPACE_H
#define BIBICONV_CODESPACE_H

#include <stdint.h>
#include <stdbool.h>
#include <bibiconv/cfeatures.h>
#include <bibiconv/chartypes.h>

#pragma mark - Tables

/**
 * Identifies the code table that a character belongs to.
 *
 * This is an index into the code space's ``bib_iconv_code_space/tables`` array.
 */
typedef uint16_t bib_iconv_table_id;

/**
 * A set of information about a code table in the target character encoding.
 *
 * The MARC-8 encoding is defined by a set of code tables defining the mapping
 * of characters to code points in an 8-bit value. These tables can be assigned
 * to one of two areas in the code space: the GL graphic area, and the GR
 * graphic area.
 *
 * This structure identifies information about a code table, such as its
 * preference to be assigned to the G0 or G1 graphic set, and the escape
 * sequence used to assign it to a graphic set.
 */
typedef struct bib_iconv_table_info {
    /**
     * The characters in an escape sequence identifying the table in the target
     * encoding.
     *
     * This is a null-terminated string of one or two characters. The empty
     * string is an invalid value.
     */
    char escape_sequence[3];

    /**
     * Indicates that the character set's code points are composed of multiple
     * code units rather than just one.
     *
     * For MARC-8, this mainly identifies the EACC (East Asian Character Codes).
     */
    bool is_multibyte : 1;

    /**
     * Indicates that code points belonging to this table should be placed in
     * the Graphic Left (GL) area when possible.
     *
     * When ``bib_iconv_table_info/requires_preference`` is `true`, code points
     * belonging to this table must be placed within the GL area.
     *
     * If ``bib_iconv_table_info/prefers_gr`` is also `true`, there is no
     * preference, and the ``requires_preference`` value is ignored.
     */
    bool prefers_gl : 1;

    /**
     * Indicates that code points belonging to this table should be placed in
     * the Graphic Right (GR) area when possible.
     *
     * When ``bib_iconv_table_info/requires_preference`` is `true`, code points
     * belonging to this table must be placed within the GR area.
     *
     * If ``bib_iconv_table_info/prefers_gl`` is also `true`, there is no
     * preference, and the ``requires_preference`` value is ignored.
     */
    bool prefers_gr : 1;

    /**
     * Indicates that the code point preference is required, and code points
     * for the table **must** be placed within the indicated graphic set.
     *
     * This value is ignored when both ``bib_iconv_table_info/prefers_gl`` and
     * ``bib_iconv_table_info/prefers_gr`` are `true`.
     */
    bool requires_preference : 1;
} bib_iconv_table_info_t;

#pragma mark - Code Points

/**
 * One possible conversion from a Unicode scalar to a code point in the target
 * character encoding.
 *
 * Multiple MARC-8 code tables, represented by ``bib_iconv_charset``s, can have
 * code points representing the same character. For example, most of the "Basic"
 * tables have code points for the same numerals and punctuation marks. It's
 * useful for the Unicode scalar to be mapped to all of these tables' code
 * points so that we can minimize the use of escape sequences to switch back and
 * forth between character sets in the converted string.
 */
typedef struct bib_iconv_candidate {
    /**
     * The code point represented by this value.
     *
     * The code point can be at most 3 code units, and must be followed by a
     * null terminator.
     *
     * For most MARC-8 character sets, code points are one code unit long. In
     * the Extended Latin (ANSEL) character set, there are
     *
     * MARC-8 characters using the EACC character set are always three code
     * units long. In the Extended Latin (ANSEL) character set, most code points
     * are one code unit long, with the exception of a couple that translate to
     * two code units. All other character sets are comprised of code points
     * that are exactly one code unit long.
     */
    codeunit8_t code_point[4];

    /**
     * Identifies the MARC-8 table that the character belongs to.
     *
     * The identified table contains information about whether or not the
     */
    bib_iconv_table_id table_id;

    /**
     * Identifies the character as a combining character.
     *
     * In the MARC-8 encoding, characters like "¨" and "ˆ" appear before the
     * character they attach to, like "ü" or "î". This field indicates that the
     * MARC-8 code point represents one such character.
     *
     * Note that MARC-8 combining characters are ordered exactly opposite from
     * how they appear in Unicode, where combining characters come after the
     * character they attach to. For example, "ä̧" in MARC-8 would be appear as
     * "`¨ ̧a`", but in Unicode the characters would appear as "`a ̧¨`".
     *
     * See [Combining characters (diacritics)][combining] from the Library of
     * Congress Specification for the MARC-8 Character set for more information
     * on combining characters.
     *
     * [combining]:
     *  http://www.loc.gov/marc/specifications/speccharconversion.html#combining
     */
    bool is_combining : 1;

    /**
     * Identifies the candidate as the best match for a Unicode code point, if
     * all other heuristics for determining the best fit are equal.
     */
    bool is_preferred_candidate : 1;

    /**
     * Identifies the candidate as valid for the represented Unicode code point.
     *
     * This field exists to differentiate between unset zeroed-out values and
     * valid assigned values within an array of ``bib_iconv_candidate`` values.
     *
     * When `is_valid_candidate` is `false`, then the candidate  should be
     * treated as an array terminator to indicate the end of an array.
     */
    bool is_valid_candidate : 1;
} bib_iconv_candidate_t;

typedef struct bib_iconv_code_entry {
    /**
     * The Unicode scalar represented by this entry in the code space.
     *
     * Use this value to sort ``bib_iconv_code_entry`` values in an array so
     * that they can be searched for using an efficient search algorithm.
     */
    ucs4_t unicode;

    /**
     * A list of candidate code points that the Unicode scalar can be
     * represented as in the target encoding.
     *
     * This array is terminated by a ``bib_iconv_candidate`` value whose
     * ``bib_iconv_candidate/is_valid_candidate`` field is `false`.
     *
     * Multiple MARC-8 code tables, represented by ``bib_iconv_charset``s, can
     * have code points representing the same character. For example, most of
     * the "Basic" tables have code points for the same numerals and punctuation
     * marks. It's useful for the Unicode scalar to be mapped to all of these
     * tables' code points so that we can minimize the use of escape sequences
     * to switch back and forth between character sets in the converted string.
     *
     * When this value is `NULL`, it indicates that the entry's unicode code
     * point cannot be converted to the target character encoding.
     */
    struct bib_iconv_candidate const *candidates;
} bib_iconv_code_entry_t;

#pragma mark - Code Pages

/**
 * A sorted set of Unicode code points that fall within a contiguous range.
 */
typedef struct bib_iconv_code_range {
    /**
     * The first Unicode code point in the bucket.
     *
     * Subtract this value from a given Unicode code point to get its index
     * within the ``bib_iconv_code_range/entries`` array.
     *
     * An array of ``bib_iconv_code_range``s can be sorted based on this value
     * to allow it to be searched through using an efficient search algorithm.
     */
    ucs4_t first;

    /**
     * The amount of Unicode code points with entries in the bucket.
     */
    uint16_t count;

    /**
     * The entries for each Unicode code point in the bucket.
     *
     * These entries are indexed by the Unicode code point minus
     * ``bib_iconv_code_range/first``. The resulting index must be less than
     * ``bib_iconv_code_range/count`` to be a valid index into this array.
     *
     * If an entry's ``bib_iconv_code_entry/candidates`` array is `NULL`, then
     * the Unicode code point cannot be converted to the target character
     * encoding.
     */
    struct bib_iconv_code_entry const *entries;
} bib_iconv_code_range_t;

/**
 * A group of Unicode code points that share the same upper 24 bits.
 *
 * For example, one code page may contain all the Unicode code points from
 * `U+0000` through `U+00FF`, and another may contain all the code points from
 * `U+4200` through `U+42FF`.
 */
typedef struct bib_iconv_code_page {
    /**
     * The high bits of the Unicode code point entries in the bucket.
     *
     * This value is used to identify the 24 high bits in the unicode code point
     * shared by all the values in the ``bib_iconv_code_page/ranges`` array.
     *
     * This value is obtained by shifting the Unicode code point to the right
     * 8 bits.
     */
    uint32_t high_bits;

    /**
     * The total amount of ranges in ``bib_iconv_code_page/ranges``.
     */
    uint32_t range_count;

    /**
     * The sets of Unicode code points that belong to the code page.
     *
     * This is a sorted array where each range is sorted by its
     * ``bib_iconv_code_range/first`` value. This allows the ranges to be
     * searched through using an efficient search algorithm.
     */
    struct bib_iconv_code_range const *ranges;
} bib_iconv_code_page_t;

#pragma mark - Code Space

/**
 * The space of all possible Unicode code points that can be converted to the
 * target character encoding.
 */
typedef struct bib_iconv_code_space {
    /**
     * The array of code tables that ``bib_iconv_candidate`` values can belong
     * to.
     */
    struct bib_iconv_table_info const *tables;

    /**
     * The length of the ``bib_iconv_code_space/tables`` array.
     */
    size_t table_count;

    /**
     * An array of Unicode code pages for Unicode characters that the code space
     * supports converting from.
     *
     * This array is sorted by each page's ``bib_iconv_code_page/high_bits``
     * value. This allows for the use of efficient search algorithms when
     * searching through the array for the correct page for a given Unicode
     * code point.
     */
    struct bib_iconv_code_page const *pages;

    /**
     * The amount of code pages in the ``bib_iconv_code_space/pages`` array.
     */
    size_t page_count;
} bib_iconv_code_space_t;

#pragma mark -

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Convert a Unicode code point into a sequence of encoded characters.
 *
 * - parameter space: The code space with the relevant conversion information.
 * - parameter unicode: The Unicode code point to convert into the target
 *   character encoding.
 * - returns: A pointer to the resulting entry describing the code point in the
 *   target character encoding, or `NULL` if the code point cannot be converted.
 *
 *  ## Example
 *
 *  ```c
 *  ucs4 thorn = 0x00FE; // þ
 *  bib_iconv_code_entry_t const *entry;
 *  entry = bib_iconv_code_space_lookup(&marc8_code_space, thorn, &entry);
 *  if (entry == nullptr) {
 *      fprintf(stderr, "Failed to convert the character from Unicode");
 *  } else {
 *      // process the resulting entry to find the best fit candidate.
 *  }
 *  ```
 */
struct bib_iconv_code_entry const *
bib_iconv_code_space_lookup(struct bib_iconv_code_space const *space,
                            ucs4_t unicode);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BIBICONV_CODESPACE_H */
