//
//  codetable.h
//  bibiconv
//

#ifndef BIBICONV_CODETABLE_H
#define BIBICONV_CODETABLE_H

#include <stddef.h>
#include <stdbool.h>
#include <bibiconv/cfeatures.h>
#include <bibiconv/chartypes.h>

#pragma mark - Trie Structure

/**
 * Maps a code unit in a coded character set to a Unicode code point.
 *
 * When placed in a lookup table, the code unit is the index where this value is
 * located in the table.
 *
 * When placed in a lookup array, the code unit is identified by the containing
 * value's ``bib_iconv_code_path/code_unit`` field.
 *
 * This code unit is either the entire value of a single-byte code point, or the
 * last code unit of a multi-byte code point.
 */
typedef struct bib_iconv_code_point {
    /**
     * Identifies the code unit as the last unit in a code point.
     *
     * When this field is `true`, this value represents either a single-byte
     * code point, or the last code unit of a multi-byte code point. In this
     * case, the `code_point` field contains the Unicode code point to
     * convert the coded character set's code point to.
     *
     * When this field is `false`, this value represents either the first code
     * unit, or a middle code unit, of a multi-byte code point. In this case,
     * the value should be treated as a ``bib_iconv_span_index_t`` value instead
     * of a ``bib_iconv_code_point_t`` value.
     *
     * > Note: This field **must always** be `true` for all values of
     *   ``bib_iconv_code_point_t``.
     */
    bool is_code_point : 1;

    /**
     * Identifies the code unit as a combining character.
     *
     * In the MARC-8 encoding, characters like "¨" and "ˆ" appear before the
     * character they attach to, like "ü" or "î". This field indicates that the
     * MARC-8 code point represents one such character.
     *
     * Note that MARC-8 combining characters are ordered exactly opposite from
     * how they appear in Unicode, where combining characters come after the
     * character they attach to. For example, the code points for "ä̧" in MARC-8
     * would be ordered as "`¨ ̧a`", but in Unicode the code points would be
     * ordered as "`a ̧¨`".
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
     * The UCS-4 Unicode code point corresponding to the code unit.
     */
    ucs4_t unicode : 30;
} bib_iconv_code_point_t;

/**
 * Maps a code unit in a coded character set to a code span with the next
 * possible code units in a multi-byte code point.
 *
 * When placed in a lookup table, the code unit is the index where this value is
 * located in the table.
 *
 * When placed in a lookup array, the code unit is identified by the containing
 * value's ``bib_iconv_code_path/code_unit`` field.
 *
 * This code unit is either the first code unit, or a middle code unit, of a
 * multi-byte code point.
 */
typedef struct bib_iconv_span_index {
    /**
     * Identifies the code unit as the last unit in a code point.
     *
     * When this field is `false`, this value represents either the first code
     * unit, or a middle code unit, of a multi-byte code point. In this case,
     * the ``bib_iconv_span_index/span_offset`` field contains the index in
     * the character sets' ``bib_iconv_charset/code_spans`` array for the
     * span table containing the next possible code units in the multibyte
     * sequence.
     *
     * When this field is `true`, this value represents either a single-byte
     * code point, or the last code unit of a multi-byte code point. In this
     * case the value should be treated as a ``bib_iconv_code_point_t``
     * value instead of a ``bib_iconv_span_index_t`` value.
     * 
     * > Note: This field **must always** be `false` for all values of
     *   ``bib_iconv_span_index_t``.
     */
    bool is_final_unit : 1;

    /**
     * Identifies the span index as valid for the code unit.
     *
     * This field exists to differentiate between unset zeroed-out values and
     * valid assigned values within an array of ``bib_iconv_code_path`` values.
     *
     * When both ``bib_iconv_span_index/is_final_unit`` and `is_valid_info` are
     * `false`, and ``bib_iconv_span_index/span_offset`` is `0`, then this value
     * should be treated as an array terminator to indicate the end of an array.
     */
    bool is_valid_info : 1;

    /**
     * The index of the code span with the code point's next possible bytes.
     *
     * This field contains the index in the character set's
     * ``bib_iconv_charset/code_spans`` array for the span table containing the
     * next possible code units from the code point's multibyte sequence.
     */
    unsigned short span_offset;
} bib_iconv_span_index_t;

/**
 * Maps a code unit in a coded character set to either a Unicode code point, or
 * a code span with the next possible code units in a multi-byte code point.
 *
 * When placed in a lookup table, the first MARC-8 code unit for this code point
 * is the index where this value is located in the table.
 *
 * When placed in a lookup array, the MARC-8 code unit is identified by the
 * array element's ``bib_iconv_code_path/code_unit``.
 *
 * Use the ``bib_iconv_code_info/is_final_unit`` field to determine which field
 * to use with this value. When `true`, use ``bib_iconv_code_info/code_point``.
 * Otherwise, use ``bib_iconv_code_info/span_index``.
 */
typedef union bib_iconv_code_info {
    /**
     * Identifies a code unit as the last unit in a code point.
     *
     * When this field is `true`, this value represents either a single-byte
     * code point, or the last code unit of a multi-byte code point. In this
     * case, the ``bib_iconv_code_info/code_point`` field contains the Unicode
     * code point to convert the coded character set's code point to.
     *
     * When this field is `false`, this value represents either the first code
     * unit, or a middle code unit, of a multi-byte code point. In this case,
     * the ``bib_iconv_code_info/span_index`` field contains information about
     * the next block of assigned units in a graphic set to use for the next
     * code unit in the sequence.
     */
    bool is_final_unit : 1;

    /**
     * Maps a code unit in a coded character set to a Unicode code point.
     */
    struct bib_iconv_code_point code_point;

    /**
     * Maps a code unit in a coded character set to a code span with the next
     * possible code units in a multi-byte code point.
     */
    struct bib_iconv_span_index span_index;
} bib_iconv_code_info_t;

#if BIBICONV_HAS_STATIC_ASSERT
static_assert(sizeof(bib_iconv_code_info_t) == sizeof(bib_iconv_code_point_t),
              "iconv map structures are not properly sized");
static_assert(sizeof(bib_iconv_code_info_t) == sizeof(bib_iconv_span_index_t),
              "iconv map structures are not properly sized");
#endif /* BIBICONV_HAS_STATIC_ASSERT */

/**
 * Identifies the mapping of a code unit to a Unicode code point in an array.
 *
 * For code sets with very few code units, the cost of wasted memory space
 * storing unused code unit information outweighs the benefits of constant-time
 * code unit lookup. In those cases, it's better to use a more space-efficient
 * flat array with O(n) lookup time.
 *
 * This value pairs together an 8-bit code unit with the map used to transform
 * it into a Unicode code point.
 *
 * Iterate over an array of ``bib_iconv_code_path`` to find the entry with the
 * desired ``bib_iconv_code_path/code_unit``. The last entry in the array should
 * have its `code_unit` set to `0x00`, acting as an array terminator to signal
 * the end of iteration. When a matching `code_unit` is found, use the
 * corresponding ``bib_iconv_code_path/code_info`` value to complete the
 * conversion.
 */
typedef struct bib_iconv_code_path {
    /**
     * The 8-bit code unit mapped to either a Unicode code point, or a code span
     * with the next possible code units in a multi-byte code point.
     *
     * Treat the value as an array terminator when this field is set to `0x00`.
     */
    codeunit8_t code_unit;

    /**
     * The value mapping the 8-bit code unit to either a Unicode code point, or
     * a code span with the next possible code units in a multi-byte code point.
     */
    union bib_iconv_code_info code_info;
} bib_iconv_code_path_t;

#pragma mark - Character Sets

/**
 * A control character set of 32 possible characters.
 *
 * See [ECMA-35 Character Code Structure and Extension Techniques][35], and
 * [ECMA-43 8-Bit Coded Character Set Structure and Rules][43] for more
 * information about the control set. They are equivalent to the ISO/IEC 2022
 * and ISO 4873 standards respectfully, but are freely available to the public.
 *
 * > Note: In accordance with ECMA-32 6.2.2 and 6.4.2, a coded character set's
 *   primary control set (C0) **must** assign the code unit `0x1B` at
 *   `lookup_table[27]` to the escape character `0x1B` (ASCII `ESC`). However,
 *   the supplementary control set (C1) **cannot** assign the corresponding code
 *   unit `0x9B` at `lookup_table[27]` to the escape character.
 *
 * [35]:
 *  https://ecma-international.org/publications-and-standards/standards/ecma-35/
 * [43]:
 *  https://ecma-international.org/publications-and-standards/standards/ecma-43/
 */
typedef struct bib_iconv_control_set {
    /**
     * A Mapping of 7-bit code units to Unicode code points.
     *
     * Given an 8-bit code unit `c`, the corresponding Unicode code point is at
     * the index `c & 0x1F`.
     *
     * > Note: In accordance with ECMA-32 6.2.2 and 6.4.2, a coded character
     *   set's primary control set (C0) **must** assign the code unit `0x1B` at
     *   `lookup_table[27]` to the escape character `0x1B` (ASCII `ESC`). 
     *   However, the supplementary control set (C1) **cannot** assign the
     *   corresponding code unit `0x9B` at `lookup_table[27]` to the escape
     *   character.
     */
    union bib_iconv_code_info lookup_table[32];
} bib_iconv_control_set_t;

/**
 * A graphic character set of 94 or 96 possible characters.
 *
 * For 94-character and 94ⁿ-character graphic sets, the first and last
 * character in ``bib_iconv_graphic_set/lookup_table`` are reserved. When placed
 * in the Graphic Left (GL) area of the code table, `lookup_table[0]` is always
 * `0x20` (ASCII space), and `lookup_table[95]` is always `0x7F` (ASCII `DEL`)
 * for these character sets.
 *
 * > Note: For simplicity of implementation, 94-character and 94ⁿ-character
 *   graphic sets **must** assign `lookup_table[0]` and `lookup_table[95]` to
 *   `0x20` (ASCII space) and `0x7F` (ASCII `DEL`) respectively.
 *
 * This is not space efficient for blocks in a 94 with only a few entries. For 
 * those cases, it may be better to use an array of ``bib_iconv_code_path_t``
 * instead.
 *
 * See [ECMA-35 Character Code Structure and Extension Techniques][35], and
 * [ECMA-43 8-Bit Coded Character Set Structure and Rules][43] for more
 * information about the graphic set. They are equivalent to the ISO/IEC 2022
 * and ISO 4873 standards respectfully, but are freely available to the public.
 *
 * [35]:
 *  https://ecma-international.org/publications-and-standards/standards/ecma-35/
 * [43]:
 *  https://ecma-international.org/publications-and-standards/standards/ecma-43/
 */
typedef struct bib_iconv_graphic_set {
    /**
     * A Mapping of 7-bit code units to Unicode code points.
     *
     * Given an 8-bit code unit `c`, the corresponding Unicode code point is at
     * the index `(c & 0x7F) - 0x20`.
     *
     * > Note: For simplicity of implementation, 94-character and 94ⁿ-character
     *   graphic sets **must** set `lookup_table[0]` and `lookup_table[95]` to
     *   `0x20` (ASCII space) and `0x7F` (ASCII `DEL`) respectively.
     */
    union bib_iconv_code_info lookup_table[96];
} bib_iconv_graphic_set_t;

/**
 * The four distinct areas of a code table for an 8-bit coded character set with
 * 256 possible characters.
 *
 * 8-bit code tables are broken up into four areas where code elements (control
 * sets and graphic sets) can be placed. These are the Control Left (CL) area,
 * the Graphic Left (GL) area, the Control Right (CR) area, and the Graphic
 * Right (GR) area.
 *
 * See [ECMA-35 Character Code Structure and Extension Techniques][35] 8.1, and
 * [ECMA-43 8-Bit Coded Character Set Structure and Rules][43] for more
 * information about code table areas. They are equivalent to the ISO/IEC 2022
 * and ISO 4873 standards respectfully, but are freely available to the public.
 *
 * [35]:
 *  https://ecma-international.org/publications-and-standards/standards/ecma-35/
 * [43]:
 *  https://ecma-international.org/publications-and-standards/standards/ecma-43/
 */
typedef struct bib_iconv_code_areas {
    /**
     * The Control Left (CL) area of the code table with code units from
     * `0x00` to `0x1F`.
     *
     * This is synonymous with the primary control set (C0).
     */
    struct bib_iconv_control_set cl;

    /**
     * The Graphic Left (GL) area of the code table with code units from
     * `0x20` to `0x7F`.
     *
     * This area may be assigned to any of the four graphic elements G0, G1, G2,
     * or G3.
     */
    struct bib_iconv_graphic_set gl;

    /**
     * The Control Right (CR) area of the code table with code units from
     * `0x80` to `0x9F`.
     *
     * This is synonymous with the supplementary control set (C1).
     */
    struct bib_iconv_control_set cr;

    /**
     * The Graphic Right (GR) area of the code table with code units from
     * `0xA0` to `0xFF`.
     *
     * This area cannot be assigned to the graphic element G0, and may only be
     * assigned to the G1, G2, or G3 elements.
     */
    struct bib_iconv_graphic_set gr;
} bib_iconv_code_areas_t;

/**
 * A code table for an 8-bit coded character set with 256 possible characters.
 *
 * This table provides fast access to the Unicode code points associated with
 * a character code point by letting you use an 8-bit code unit as the index to
 * a Unicode code point in the ``bib_iconv_code_table/lookup_table``.
 *
 * [https://www.loc.gov/marc/specifications/speccharmarc8.html][1]
 *
 * [1]: https://www.loc.gov/marc/specifications/speccharmarc8.html
 *
 * See [ECMA-35 Character Code Structure and Extension Techniques][35], and
 * [ECMA-43 8-Bit Coded Character Set Structure and Rules][43] for more
 * information about code tables. They are equivalent to the ISO/IEC 2022 and
 * ISO 4873 standards respectfully, but are freely available to the public.
 *
 * [35]:
 *  https://ecma-international.org/publications-and-standards/standards/ecma-35/
 * [43]:
 *  https://ecma-international.org/publications-and-standards/standards/ecma-43/
 */
typedef union bib_iconv_code_table {
    /**
     * A mapping of 8-bit code units (as the index), to Unicode code points.
     */
    union bib_iconv_code_info lookup_table[256];

    /**
     * The four distinct areas of a code table for an 8-bit coded character set
     * with 256 possible characters.
     *
     * 8-bit code tables are broken up into four areas where code elements 
     * (control sets and graphic sets) can be placed. These are the Control Left
     * (CL) area, the Graphic Left (GL) area, the Control Right (CR) area, and
     * the Graphic Right (GR) area.
     *
     * ## See Also
     *
     * ### Code Table Areas
     *
     * - ``bib_iconv_code_areas``
     * - ``bib_iconv_code_areas/cl``
     * - ``bib_iconv_code_areas/gl``
     * - ``bib_iconv_code_areas/cr``
     * - ``bib_iconv_code_areas/gr``
     */
    struct bib_iconv_code_areas code_areas;
} bib_iconv_code_table_t;

/**
 * Represents a slice of code units within a 94ⁿ-character or 96ⁿ-character
 * graphic set.
 *
 * For slices of a multibyte character set with very few code units, the cost of
 * wasted memory space storing information about unassigned code units outweighs
 * the benefits of constant-time lookup. In those cases, it's better to use a
 * more space-efficient flat array with O(n) lookup time.
 *
 * This structure provides flexibility in representation by allowing slices of
 * code units within a multibyte graphic set to be stored as either a complete
 * ``bib_iconv_graphic_set_t`` with constant-time lookup, or an array of
 * ``bib_iconv_code_path_t`` elements for space-efficient storage of slices with
 * few assigned code units.
 *
 * Iterate over an array of `bib_iconv_code_path_t` to find the entry with the
 * desired ``bib_iconv_code_path/code_unit``. The last entry in the array should
 * have its `code_unit` set to `0x00`.
 *
 * Either ``bib_iconv_code_span/graphic_set`` or
 * ``bib_iconv_code_span/graphic_map`` will be non-null, but never both. This
 * ensures that each code span is represented in the most efficient manner
 * possible.
 *
 * See [ECMA-35 Character Code Structure and Extension Techniques][35], and
 * [ECMA-43 8-Bit Coded Character Set Structure and Rules][43] for more
 * information about multibyte character sets. They are equivalent to the
 * ISO/IEC 2022 and ISO 4873 standards respectfully, but are freely available to
 * the public.
 *
 * [35]:
 *  https://ecma-international.org/publications-and-standards/standards/ecma-35/
 * [43]:
 *  https://ecma-international.org/publications-and-standards/standards/ecma-43/
 */
typedef struct bib_iconv_code_span {
    /**
     * A slice of code units with constant-time lookup.
     *
     * Use this field when the block of code units is best represented by a
     * complete graphic set with direct lookup capabilities.
     *
     * This field is `NULL` when ``bib_iconv_code_span/graphic_map`` is used.
     */
    struct bib_iconv_graphic_set const *graphic_set;

    /**
     * A space-efficient array of assigned units for a slice of a graphic set
     * with few assignments.
     *
     * Use this field when the block of code units is sparse or irregular,
     * making it more efficient to use a flat array for conversion.
     *
     * Iterate over this array of ``bib_iconv_code_path`` to find the entry with
     * the desired ``bib_iconv_code_path/code_unit``. The last entry in the
     * array should have its `code_unit` set to `0x00`, acting as an array
     * terminator to signal the end of iteration. When a matching `code_unit` is
     * found, use the corresponding ``bib_iconv_code_path/code_info`` value to
     * complete the conversion.
     *
     * This field is `NULL` when ``bib_iconv_code_span/graphic_set`` is used.
     */
    struct bib_iconv_code_path const *graphic_map;
} bib_iconv_code_span_t;

/**
 * A set of assigned code points composed of 8-bit code units mapped to their
 * equivalent Unicode code points.
 *
 * See [ECMA-35 Character Code Structure and Extension Techniques][35], and
 * [ECMA-43 8-Bit Coded Character Set Structure and Rules][43] for more
 * information about coded character sets. They are equivalent to the ISO/IEC
 * 2022 and ISO 4873 standards respectfully, but are freely available to the
 * public.
 *
 * [35]:
 *  https://ecma-international.org/publications-and-standards/standards/ecma-35/
 * [43]:
 *  https://ecma-international.org/publications-and-standards/standards/ecma-43/
 */
typedef struct bib_iconv_charset {
    /**
     * The table of all characters in the character set.
     */
    union bib_iconv_code_table code_table;

    /**
     * An array of code spans referenced by multi-byte sequences in
     * ``code_table``.
     *
     * There's only one multi-byte code table, so this value will be `NULL` for
     * all character sets except for those containing EACC characters. For this
     * reason, the only valid non-`NULL` value for this field is that of
     * ``bib_iconv_marc8_eacc``'s ``code_spans`` field. Any other values will
     * result in an error.
     *
     * - note: It is not possible to get the length of this array. Only the EACC
     *   character set is aware of the valid indexes into this array. Do not
     *   attempt to access values from this array without consulting the EACC
     *   code table.
     */
    struct bib_iconv_code_span const *code_spans;
} bib_iconv_charset_t;

#pragma mark -

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Convert a sequence of encoded code units into a Unicode code point.
 *
 * - parameter charset: The character set structure with the relevant Unicode
 *   conversion information.
 * - parameter length: The total amount of code units in `input`.
 * - parameter input: A string of code units to convert.
 * - parameter location: The index of the code unit in `input` to treat as the
 *   beginning of an encoded code point. This value is set to the index of the
 *   next code unit in the string, or the end of the string, after a successful
 *   conversion.
 * - parameter unicode: The location to store the resulting Unicode code point.
 * - parameter is_combining: The location to store a flag indicating whether or
 *   not the converted code point represents a combining character.
 * - returns: `true` when the a code point is successfully converted. Otherwise
 *   returns `false`.
 *
 * ## Errors
 *
 * When this function returns `false`, `errno` is set to a value identifying the
 * reason for the conversion failure.
 * 
 * - term **`EINVAL`**: Either `charset`, `input`, or `location` are `NULL`; or
 *   `length` is less than or equal to `*location`.
 * - term **`EILSEQ`**: The code units starting at `*location` do not represent
 *   a valid character in `charset`.
 *
 *  ## Example
 *
 *  ```c
 *  char const input[] = { 0x23, 0x28, 0x22, 0x00 };
 *  size_t location = 0;
 *  ucs4_t unicode = 0;
 *  bool is_combining = false;
 *  if (bib_iconv_charset_lookup(&bib_iconv_marc8_eacc,
 *                               sizeof(input), input, &location,
 *                               &unicode, &is_combining)) {
 *      if (unicode != UCS4_IGNORED) {
 *          // Process the resulting unicode code point.
 *      }
 *  } else {
 *      perror("Failed to convert character sequence to unicode.");
 *  }
 *  ```
 */
bool bib_iconv_charset_lookup(struct bib_iconv_charset const *charset,
                              size_t length, char const input[length],
                              size_t *location, ucs4_t unicode[1],
                              bool *is_combining);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BIBICONV_CODETABLE_H */
