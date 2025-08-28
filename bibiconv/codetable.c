//
//  codetable.c
//  bibiconv
//

#include "cfeatures.h"
#include "codetable.h"
#include "cstderrno.h"

/**
 * Inspect a ``bib_iconv_code_info`` value to get the code point from a final
 * code unit, or follow a span index to lookup the next code unit in a multibyte
 * sequence.
 *
 * - parameter charset: The character set structure with the relevant Unicode
 *   conversion information.
 * - parameter map: The code unit information to inspect.
 * - parameter length: The total amount of code units in `input`.
 * - parameter input: A string of code units to convert.
 * - parameter location: The index of the code unit in input to treat as the
 *   current code point being looked up. This value is set to the index of the
 *   next code unit in the string, or the end of the string, after a successful
 *   conversion.
 * - parameter unicode: The location to store the resulting Unicode code point.
 * - parameter is_combining: The location to store a flag indicating whether or
 *   not the converted code point represents a combining character.
 * - returns: `true` when the a code point is successfully converted. Otherwise
 *   returns `false`.
 *
 * When given a `map` that's the final code unit in a multibyte sequence, or
 * the code point for a single byte, this function sets the `unicode` and
 * `is_combining` parameters and returns with a successful conversion.
 *
 * When given a `map` that's the first or middle byte in a multibyte sequence,
 * this function follows the span index and looks up the next code unit in the
 * `input` string in the next span's table.
 */
static bool lookup_info(struct bib_iconv_charset const *charset,
                        union bib_iconv_code_info map,
                        size_t length, char const input[length],
                        size_t *location, ucs4_t *unicode, bool *is_combining);

/**
 * Inspect a ``bib_iconv_code_span`` value to get the code point from a final
 * code unit, or follow a span index to lookup the next code unit in a multibyte
 * sequence.
 *
 * - parameter charset: The character set structure with the relevant Unicode
 *   conversion information.
 * - parameter span: The code span information to inspect.
 * - parameter length: The total amount of code units in `input`.
 * - parameter input: A string of code units to convert.
 * - parameter location: The index of the code unit in input to treat as the
 *   current code point being looked up. This value is set to the index of the
 *   next code unit in the string, or the end of the string, after a successful
 *   conversion.
 * - parameter unicode: The location to store the resulting Unicode code point.
 * - parameter is_combining: The location to store a flag indicating whether or
 *   not the converted code point represents a combining character.
 * - returns: `true` when the a code point is successfully converted. Otherwise
 *   returns `false`.
 */
static bool lookup_block(struct bib_iconv_charset const *charset,
                         struct bib_iconv_code_span span,
                         size_t length, char const input[length],
                         size_t *location, ucs4_t *unicode, bool *is_combining);

bool bib_iconv_charset_lookup(struct bib_iconv_charset const *charset,
                              size_t length, char const input[length],
                              size_t *location, ucs4_t *unicode,
                              bool *is_combining) {
    if (charset == nullptr || input == nullptr || location == nullptr) {
        errno = EINVAL;
        return false;
    }
    if (length <= *location) {
        errno = EINVAL;
        return false;
    }
    /**
     * A sentinel span looking up nothing.
     *
     * `{ NULL, NULL }` is a sentinel value that has ``lookup_block`` lookup the
     * initial ``bib_iconv_code_info`` value from the charset's code table.
     */
    struct bib_iconv_code_span span = { nullptr, nullptr };
    return lookup_block(charset, span, length, input, location, unicode,
                        is_combining);
}

static bool lookup_info(struct bib_iconv_charset const *charset,
                        union bib_iconv_code_info info,
                        size_t len, char const input[len], size_t *location,
                        ucs4_t *unicode, bool *combining) {
    if (len <= *location) {
        errno = EILSEQ;
        return false;
    }
    if (info.is_final_unit) {
        /* The code unit is the final unit in a multibyte sequence, or is the
         * code point for a single byte character. */
        if (info.code_point.unicode == UCS4_NONCHAR) {
            errno = EILSEQ;
            return false;
        }
        if (unicode != nullptr) {
            *unicode = info.code_point.unicode;
        }
        if (combining != nullptr) {
            *combining = info.code_point.is_combining;
        }
        *location += 1;
        return true;
    } else {
        /* The code unit is the first or middle byte in a multibyte sequence. */
        /* Lookup the next span to lookup the next code unit in the sequence. */
        size_t loc = *location + 1;
        size_t index = info.span_index.span_offset;
        struct bib_iconv_code_span span = charset->code_spans[index];
        if (lookup_block(charset, span, len, input, &loc, unicode, combining)) {
            *location = loc;
            return true;
        }
        return false;
    }
}

static bool lookup_block(struct bib_iconv_charset const *charset,
                         struct bib_iconv_code_span span,
                         size_t len, char const input[len], size_t *location,
                         ucs4_t *unicode, bool *combining) {
    if (len <= *location) {
        errno = EILSEQ;
        return false;
    }
    if (span.graphic_set != nullptr) {
        /* The span is a constant-time lookup table of code units. */
        /* The lookup table only contains code points from 0x20 to 0x7F. */
        /* Normalize the code unit to lookup the character within that range. */
        unsigned char unit = input[*location] & 0x7F;
        if (unit < 0x20) {
            /* The graphic set never contains values less than 0x20. */
            errno = EILSEQ;
            return false;
        }
        /* Shift the code unit down by 0x20 get the index of the code unit
         * information in the lookup table. */
        unit -= 0x20;
        size_t loc = *location;
        /* Lookup the code unit information from the lookup table. */
        union bib_iconv_code_info info = span.graphic_set->lookup_table[unit];
        /* Read the code unit information and follow it to continue reading
         * the complete code point from the input string. */
        if (lookup_info(charset, info, len, input, &loc, unicode, combining)) {
            *location = loc;
            return true;
        }
        return false;
    } else if (span.graphic_map != nullptr) {
        /* The span is an O(n) time lookup array of code units. */
        unsigned char unit = input[*location] & 0x7F;
        struct bib_iconv_code_path const *map = span.graphic_map;
        /* Iterate over the array of code unit information to find the one
         * appropriate for the current code unit. */
        for (int index = 0; map[index].code_unit != 0x00; index += 1) {
            if (map[index].code_unit != unit) {
                /* This isn't the code unit information we're looking for. */
                continue;
            }
            /* We found the information we're looking for. */
            size_t loc = *location;
            /* Lookup the code unit information at the current index. */
            union bib_iconv_code_info info = map[index].code_info;
            /* Read the code unit information and follow it to continue reading
             * the complete code point from the input string. */
            if (lookup_info(charset, info, len, input, &loc, unicode,
                            combining)) {
                *location = loc;
                return true;
            }
            return false;
        }
        errno = EILSEQ;
        return false;
    } else {
        /* The span is a sentinel indicating the start of a lookup sequence. */
        size_t loc = *location;
        unsigned char unit = input[*location];
        /* Perform the initial lookup in the charset's code table to get the
         * appropriate code info for the current code unit. */
        union bib_iconv_code_info info = charset->code_table.lookup_table[unit];
        /* Read the code unit information and follow it to continue reading
         * the complete code point from the input string. */
        if (lookup_info(charset, info, len, input, &loc, unicode, combining)) {
            *location = loc;
            return true;
        }
        return false;
    }
}
