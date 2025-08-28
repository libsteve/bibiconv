//
// marc8_escapes.c
// bibiconv
//

#include "marc8_tables.h"
#include "iconv_coding.h"
#include <errno.h>
#include <assert.h>
#include <stdlib.h>

#define ESCAPE_CHAR 0x1B

/** The stateful data used by the MARC-8 decoder. */
typedef struct context {
    bib_iconv_charset_t working_set;

    /** An array of combining characters read from the input buffer. */
    ucs4_t *combining_chars;

    /** The allocated size of ``combining_chars``. */
    size_t combining_cap;

    /** The amount of characters set in ``combining_chars``. */
    size_t combining_len;
} context_t;

/**
 * Read an escape sequence from the `input` string of code units, and shift the
 * working set `charset` in the appropriate code area into the character set
 * identified by the shift sequence.
 *
 * - parameter charset: The working set used to lookup code points.
 * - parameter length: The length of the input string.
 * - parameter input: A string of code units containing the escape sequence.
 * - parameter location: The current location in the `input` string. The
 *   location must point to the escape character (`0x1B`, `'\e'`) at the
 *   beginning of the escape sequence. This value will point to the first code
 *   unit after the escape sequence upon reading a valid escape sequence.
 * - returns: `true` after successfully shifting the working set into the
 *   character set identified by the escape sequence. false` when the escape
 *   sequence is invalid or cut short at the end of the input string.
 */
static bool marc8_read_escape(struct bib_iconv_charset *charset,
                              size_t length, char const input[length],
                              size_t *location) {
    if (length == 0 || input == nullptr || location == nullptr) {
        errno = EINVAL;
        return false;
    }

    /** The current location in input to read characters from. */
    size_t loc = *location;

    if ((loc < length) && input[loc] == ESCAPE_CHAR) {
        loc += 1;
    } else {
        /* Invalid argument. Not an escape sequence. */
        errno = EINVAL;
        return false;
    }

    if (loc < length) {
        switch (input[loc]) {
            case 0x67: /* 'g' is G0 Greek symbol set. */
                charset->code_table.code_areas.gl =
                    bib_iconv_marc8_greek_symbols.code_table.code_areas.gl;
                *location = loc + 1;
                return true;

            case 0x62: /* 'b' is G0 Subscript set. */
                charset->code_table.code_areas.gl =
                    bib_iconv_marc8_subscript.code_table.code_areas.gl;
                *location = loc + 1;
                return true;

            case 0x70: /* 'p' is G0 Superscript set. */
                charset->code_table.code_areas.gl =
                    bib_iconv_marc8_superscript.code_table.code_areas.gl;
                *location = loc + 1;
                return true;

            case 0x73: /* 's' is G0 ASCII  */
                charset->code_table.code_areas.gl =
                    bib_iconv_marc8_basic_latin.code_table.code_areas.gl;
                *location = loc + 1;
                return true;

            /* Valid first intermediate bytes: */
            case 0x24: /* '$' denotes multibyte characters. */
            case 0x28: /* '(' sets the G0 code page. */
            case 0x2C: /* ',' sets the G0 code page. */
            case 0x29: /* ')' sets the G1 code page. */
            case 0x2D: /* '-' sets the G1 code page. */
                break;

            default:
                /* Invalid escape sequence. */
                errno = EILSEQ;
                return false;
        }
    }

    /**
     * The control set that may be modified by the escape sequence.
     */
    struct bib_iconv_control_set *control = nullptr;
    /**
     * The graphic set that will be modified by the escape sequence.
     */
    struct bib_iconv_graphic_set *graphic = nullptr;
    /**
     * Indicates that the escape sequence should identify the Extended Latin
     * character set.
     */
    bool expect_ansel_encoding = false;
    /**
     * Indicates that the escape sequence identifies a multibyte character set.
     *
     * \note The only valid multibyte character set for MARC-8 is EACC set.
     * \note Multibyte sequence implies the G0 code area.
     */
    bool is_multibyte_sequence = false;
    /**
     * Indicates that a multibyte sequence implies the G0 code area.
     */
    bool is_g0_implied = false;
    /**
     * Indicates that the G0 code area will be modified by the escape sequence.
     */
    bool is_g0 = false;
    /**
     * Indicates that the G1 code area will be modified by the escape sequence.
     */
    bool is_g1 = false;

    /* Intermediate bytes are 0x20-0x2F */
    while ((loc < length) && (input[loc] & 0xF0) == 0x20) {
        switch (input[loc]) {
            case 0x21: /* '!' is a special-case for ANSEL. */
                /* This must come after a code page indicator. */
                if (graphic == nullptr) {
                    errno = EILSEQ;
                    return false;
                }
                /* ASNEL is not a multibyte code set. */
                /* Cannot have multiple ANSEL markers in a row. */
                if (is_multibyte_sequence || expect_ansel_encoding) {
                    errno = EILSEQ;
                    return false;
                }
                loc += 1;
                expect_ansel_encoding = true;
                continue;

            case 0x24: /* '$' denotes multibyte characters. */
                /* This must precede a code page indicator. */
                /* You can only have one multibyte indicator. */
                if (graphic != nullptr || is_multibyte_sequence) {
                    errno = EILSEQ;
                    return false;
                }
                loc += 1;
                /* GL code area is implied. */
                graphic = &charset->code_table.code_areas.gl;
                is_multibyte_sequence = true;
                is_g0_implied = true;
                continue;

            case 0x28: /* '(' sets the G0 code page. */
            case 0x2C: /* ',' sets the G0 code page. */
                if (graphic != nullptr && !is_multibyte_sequence) {
                    /* You can only have one page indicator. */
                    errno = EILSEQ;
                    return false;
                }
                if (is_g0 || is_g1) {
                    /* You can only have one page indicator. */
                    errno = EILSEQ;
                    return false;
                }
                loc += 1;
                graphic = &charset->code_table.code_areas.gl;
                is_g0 = true;
                continue;

            case 0x29: /* ')' sets the G1 code page. */
            case 0x2D: /* '-' sets the G1 code page. */
                if (graphic != nullptr && !is_multibyte_sequence) {
                    /* You can only have one page indicator. */
                    errno = EILSEQ;
                    return false;
                }
                if (is_g0 || is_g1) {
                    /* You can only have one page indicator. */
                    errno = EILSEQ;
                    return false;
                }
                loc += 1;
                graphic = &charset->code_table.code_areas.gr;
                control = &charset->code_table.code_areas.cr;
                is_g1 = true;
                continue;

            default: /* Unrecognized intermediate byte. */
                errno = EILSEQ;
                return false;
        }
    }

    /* Make sure the implied G0 character set is used only when the G1 or G1
     * code areas haven't been explicitly identified in the escape sequence. */
    if (is_g0_implied && !(is_g0 || is_g1)) {
        is_g0 = true;
    }

    /* Asset that only one graphic set is identified by the escape sequence. */
    /* This should be taken care of by the checks in the while loop, so this
     * assert is only here for a sanity check during debug. */
    assert((is_g0 || is_g1) && !(is_g0 && is_g1));

    if (loc >= length) {
        /* Premature end of escape sequence. */
        errno = EILSEQ;
        return false;
    }

    /** The new character set identified by the escape sequence. */
    struct bib_iconv_charset const *new_charset = nullptr;

    /* Lookup the character set based on the final character. */
    switch (input[loc]) {
        case 0x33: /* '3' is Basic Arabic */
            if (expect_ansel_encoding || is_multibyte_sequence) {
                /* Basic Arabic must not have the intermediate '!'. */
                /* Basic Arabic is not a multibyte character set. */
                errno = EILSEQ;
                return false;
            }
            new_charset = &bib_iconv_marc8_basic_arabic;
            break;

        case 0x34: /* '4' is Extended Arabic */
            if (expect_ansel_encoding || is_multibyte_sequence) {
                /* Extended Arabic must not have the intermediate '!'. */
                /* Extended Arabic is not a multibyte character set. */
                errno = EILSEQ;
                return false;
            }
            new_charset = &bib_iconv_marc8_extended_arabic;
            break;

        case 0x42: /* 'B' is Basic Latin (ASCII). */
            if (expect_ansel_encoding || is_multibyte_sequence) {
                /* ASCII must not have the intermediate '!'. */
                /* ASCII is not a multibyte character set. */
                errno = EILSEQ;
                return false;
            }
            new_charset = &bib_iconv_marc8_basic_latin;
            break;

        case 0x45: /* 'E' is Extended Latin (ANSEL). */
            if (!expect_ansel_encoding || is_multibyte_sequence) {
                /* ANSEL encoding is special and requires intermediate '!'. */
                /* ANSEL is not a multibyte character set. */
                errno = EILSEQ;
                return false;
            }
            new_charset = &bib_iconv_marc8_extended_latin;
            break;

        case 0x31: /* '1' is Chinese, Japanese, Korean (EACC). */
            if (expect_ansel_encoding || !is_multibyte_sequence) {
                /* EACC must not have the intermediate '!'. */
                /* EACC is a multibyte character set. */
                errno = EILSEQ;
                return false;
            }
            new_charset = &bib_iconv_marc8_eacc;
            break;

        case 0x4E: /* 'N' is Basic Cyrillic. */
            if (expect_ansel_encoding || is_multibyte_sequence) {
                /* Basic Cyrillic must not have the intermediate '!'. */
                /* Basic Cyrillic is not a multibyte character set. */
                errno = EILSEQ;
                return false;
            }
            new_charset = &bib_iconv_marc8_basic_cyrillic;
            break;

        case 0x51: /* 'Q' is Extended Cyrillic. */
            if (expect_ansel_encoding || is_multibyte_sequence) {
                /* Extended Cyrillic must not have the intermediate '!'. */
                /* Extended Cyrillic is not a multibyte character set. */
                errno = EILSEQ;
                return false;
            }
            new_charset = &bib_iconv_marc8_extended_cyrillic;
            break;

        case 0x53: /* 'S' is Basic Greek. */
            if (expect_ansel_encoding || is_multibyte_sequence) {
                /* Basic Greek must not have the intermediate '!'. */
                /* Basic Greek is not a multibyte character set. */
                errno = EILSEQ;
                return false;
            }
            new_charset = &bib_iconv_marc8_basic_greek;
            break;

        case 0x32: /* '2' is Basic Hebrew. */
            if (expect_ansel_encoding || is_multibyte_sequence) {
                /* Basic Hebrew must not have the intermediate '!'. */
                /* Basic Hebrew is not a multibyte character set. */
                errno = EILSEQ;
                return false;
            }
            new_charset = &bib_iconv_marc8_basic_hebrew;
            break;

        default: /* No other character sets are supported. */
            errno = EILSEQ;
            return false;
    }
    loc += 1;

    if (is_g0) {
        *graphic = new_charset->code_table.code_areas.gl;
    } else if (is_g1) {
        assert(control != nullptr);
        *control = new_charset->code_table.code_areas.cr;
        *graphic = new_charset->code_table.code_areas.gr;
    }
    if (is_multibyte_sequence) {
        charset->code_spans = new_charset->code_spans;
    }

    *location = loc;
    return true;
}

static size_t marc8_read(bib_iconv_t cd, bib_iconv_decoder_t d,
                         char const **restrict src, size_t *restrict srcleft,
                         ucs4_t *unicode) {
    assert(cd != nullptr);
    assert(d != nullptr);
    assert(src != nullptr);
    assert(*src != nullptr);
    assert(srcleft != nullptr);
    assert(d->context != nullptr);
    context_t *context = (context_t *)d->context;

    if (context->combining_len > 0) {
        /* If there are any combining characters, return them first in reverse
         * order before going on to read any more code points. */
        context->combining_len -= 1;
        *unicode = context->combining_chars[context->combining_len];
        return 0;
    }

    unsigned char c = **src;
    while (c == ESCAPE_CHAR) {
        /* Read all escape sequences and perform the necessary shifts of the
         * working character set before attempting to read any code units. */
        char const *const initial_src = *src;
        size_t const initial_srcleft = *srcleft;
        size_t len = *srcleft;
        size_t loc = 0;
        if (!marc8_read_escape(&context->working_set, len, *src, &loc)) {
            *src = initial_src;
            *srcleft = initial_srcleft;
            return (size_t)-1;
        }
        *src += loc;
        *srcleft -= loc;
        c = **src;
    }

    /* We use a while loop here because we want to consume any and all combining
     * characters before we go on to return a UCS-4 code point. This is because
     * combining characters come after the code point they modify in Unicode,
     * but come before the code point they modify in MARC-8. */
    while (c != 0) {
        char const *const initial_src = *src;
        size_t const initial_srcleft = *srcleft;
        size_t len = *srcleft;
        size_t loc = 0;
        ucs4_t uni = 0;
        bool is_combining = true;
        /* Lookup the next code point from the working character set. */
        if (!bib_iconv_charset_lookup(&context->working_set,
                                      len, *src, &loc, &uni, &is_combining)) {
            *src = initial_src;
            *srcleft = initial_srcleft;
            return (size_t)-1;
        }
        *src += loc;
        *srcleft -= loc;
        c = **src;
        if (uni == UCS4_IGNORED) {
            /* The character is represented by two code points in MARC-8, but
             * only one code point in Unicode, so we ignore the second code
             * point and go on to read the next code point in the string. */
            continue;
        } else if (!is_combining) {
            /* The character is not a combining character, so we return it. */
            *unicode = uni;
            return 0;
        }
        if (context->combining_len >= context->combining_cap) {
            /* Make sure we resize the combining character array to contain
             * all combining characters encountered thus far. */
            /* Assert that there's always room for more than one character in
             * the combining character array. This is initially set by the
             * init function with the value of 8, but this debug assert is here
             * as a sanity check just in case. */
            assert(context->combining_cap > 1);
            /* Extend the the combining character array's capacity by half. */
            context->combining_cap += context->combining_cap / 2;
            size_t new_size = context->combining_cap * sizeof(ucs4_t);
            ucs4_t *combiners = realloc(context->combining_chars, new_size);
            if (combiners == nullptr) {
                /* We ran out of memory. Exit the function with an error. */
                *src = initial_src;
                *srcleft = initial_srcleft;
                errno = ENOMEM;
                return (size_t)-1;
            }
            context->combining_chars = combiners;
        }
        /* Add the Unicode code point to the end of the combining characters
         * array. */
        context->combining_chars[context->combining_len] = uni;
        context->combining_len += 1;
    }

    *unicode = 0;
    return 0;
}

static size_t marc8_init(bib_iconv_t cd, bib_iconv_decoder_t d,
                         char const **restrict src, size_t *restrict srcleft) {
    context_t *context = (context_t *)d->context;
    if (context == nullptr) {
        context = calloc(1, sizeof(context_t));
        d->context = context;
    }
    /* MARC-8 always begins decoding with the ASCII and ASNEL character sets in
     * the working set. */
    context->working_set.code_table.code_areas.cl =
        bib_iconv_marc8_basic_latin.code_table.code_areas.cl;
    context->working_set.code_table.code_areas.gl =
        bib_iconv_marc8_basic_latin.code_table.code_areas.gl;
    context->working_set.code_table.code_areas.cr =
        bib_iconv_marc8_extended_latin.code_table.code_areas.cr;
    context->working_set.code_table.code_areas.gr =
        bib_iconv_marc8_extended_latin.code_table.code_areas.gr;
    context->working_set.code_spans = bib_iconv_marc8_eacc.code_spans;
    context->combining_cap = 8;
    context->combining_len = 0;
    context->combining_chars = calloc(context->combining_cap, sizeof(ucs4_t));
    return 0;
}

static void marc8_deinit(bib_iconv_decoder_t d) {
    context_t *context = (context_t *)d->context;
    free(context->combining_chars);
    free(context);
}

void bib_iconv_open_marc8_decoder(bib_iconv_decoder_t d) {
    d->init = marc8_init;
    d->read = marc8_read;
    d->deinit = marc8_deinit;
}
