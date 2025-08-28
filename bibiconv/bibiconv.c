//
//  bibiconv.c
//  bibiconv
//

#include "bibiconv.h"
#include "cfeatures.h"
#include "chartypes.h"
#include "codetable.h"
#include "marc8_tables.h"
#include "iconv_coding.h"
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

struct bib_iconv_s {
    struct bib_iconv_encoder_s encoder;
    struct bib_iconv_decoder_s decoder;
    ucs4_t pending_write;
    bool has_pending_write;
    bool is_initialized;
};

/**
 * Test a given encoding code against an expected value.
 *
 * - parameter given: The encoding code to test.
 * - parameter expected: The encoding code to test against.
 * - returns: `0` when the given code matches the expected code, `-1` when the
 *   given code is ordered before the expected code, and `1` when the given code
 *   is ordered after the expected code.
 *
 * This function ignores `'-'` characters, and is case insensitive.
 */
static int matchcode(char const *given, char const *expected) {
    while (*given != 0 && *expected != 0) {
        unsigned char c1 = tolower(*given);
        unsigned char c2 = tolower(*expected);
        if (c1 == '-') {
            given += 1;
            continue;
        }
        if (c2 == '-') {
            expected += 1;
            continue;
        }
        if (c1 != c2) {
            break;
        }
        given += 1;
        expected += 1;
    }
    if (*given == *expected) {
        return 0;
    } else if (*given < *expected) {
        return -1;
    } else {
        return 1;
    }
}

bib_iconv_t bib_iconv_open(char const *restrict to, char const *restrict from) {
    if (to == nullptr || strcmp(to, "") == 0) {
        to = "char";
    }
    if (from == nullptr || strcmp(from, "") == 0) {
        from = "char";
    }

    bib_iconv_t cd = calloc(1, sizeof(struct bib_iconv_s));
    if (cd == nullptr) {
        errno = ENOMEM;
        return (bib_iconv_t)-1;
    }

    if (matchcode(to, "UTF-8") == 0 || matchcode(to, "char") == 0) {
        bib_iconv_open_utf8_encoder(&cd->encoder);
    } else if (matchcode(to, "UTF=16") == 0 || matchcode(to, "UCS-2") == 0) {
        bib_iconv_open_utf16_encoder(&cd->encoder);
    } else if (matchcode(to, "UTF-32") == 0 || matchcode(to, "UCS-4") == 0
               || matchcode(to, "wchar") == 0
               || matchcode(to, "wchar_t") == 0) {
        bib_iconv_open_utf32_encoder(&cd->encoder);
    } else {
        free(cd);
        errno = EINVAL;
        return (bib_iconv_t)-1;
    }

    if (matchcode(from, "MARC-8") == 0 || matchcode(from, "ANSEL") == 0) {
        bib_iconv_open_marc8_decoder(&cd->decoder);
    } else if (matchcode(from, "UTF-8") == 0 || matchcode(from, "char") == 0) {
        bib_iconv_open_utf8_decoder(&cd->decoder);
    } else if (matchcode(from, "UTF-16") == 0
               || matchcode(from, "UCS-2") == 0) {
        bib_iconv_open_utf16_decoder(&cd->decoder);
    } else if (matchcode(from, "UTF-32") == 0 || matchcode(from, "UCS-4") == 0
               || matchcode(from, "wchar") == 0
               || matchcode(from, "wchar_t") == 0) {
        bib_iconv_open_utf32_decoder(&cd->decoder);
    } else {
        free(cd);
        errno = EINVAL;
        return (bib_iconv_t)-1;
    }

    cd->is_initialized = false;
    return cd;
}

size_t bib_iconv(bib_iconv_t cd,
                 char const **restrict src, size_t *restrict srcleft,
                 char **restrict dst, size_t *restrict dstleft) {
    if (cd == nullptr) {
        errno = EINVAL;
        return (size_t)-1;
    }

    if (!cd->is_initialized) {
        if (cd->encoder.init != nullptr) {
            cd->encoder.init(&cd->encoder);
        }
        if (cd->decoder.init != nullptr) {
            size_t result = cd->decoder.init(cd, &cd->decoder, src, srcleft);
            if (result == (size_t)-1) {
                return result;
            }
        }
        cd->is_initialized = true;
    }

    if (src == nullptr || *src == nullptr) {
        if (dst != nullptr && *dst != nullptr) {
            size_t result = 0;
            if (cd->has_pending_write) {
                size_t r = cd->encoder.write(cd, &cd->encoder,
                                             cd->pending_write, dst, dstleft);
                if (r == (size_t)-1) {
                    return r;
                }
                result += r;
                cd->pending_write = 0;
                cd->has_pending_write = false;
            }
            if (cd->encoder.flush != nullptr) {
                size_t r = cd->encoder.flush(cd, &cd->encoder, dst, dstleft);
                if (r == (size_t)-1) {
                    return r;
                }
                result += r;
            }
            cd->is_initialized = false;
            return result;
        } else {
            cd->pending_write = 0;
            cd->has_pending_write = false;
            cd->is_initialized = false;
            return 0;
        }
    }

    if (srcleft == nullptr || *srcleft == 0) {
        return 0;
    }

    if (dst == nullptr || *dst == nullptr || dstleft == nullptr) {
        errno = EINVAL;
        return (size_t)-1;
    }

    size_t accumulated_result = 0;
    while (true) {
        ucs4_t uni = 0;

        if (cd->has_pending_write) {
            uni = cd->pending_write;
            cd->pending_write = 0;
            cd->has_pending_write = false;
        } else {
            size_t r = cd->decoder.read(cd, &cd->decoder, src, srcleft, &uni);
            if (r == (size_t)-1) {
                return r;
            }
        }

        size_t result = cd->encoder.write(cd, &cd->encoder, uni, dst, dstleft);
        if (result == (size_t)-1) {
            cd->pending_write = uni;
            cd->has_pending_write = true;
            return result;
        }

        accumulated_result += result;
        if (uni == 0 || *dstleft == 0) {
            return accumulated_result;
        }
    }
}

int bib_iconv_close(bib_iconv_t cd) {
    if (cd == nullptr) {
        return 0;
    }
    if (cd->encoder.deinit != nullptr) {
        cd->encoder.deinit(&cd->encoder);
    }
    if (cd->decoder.deinit != nullptr) {
        cd->decoder.deinit(&cd->decoder);
    }
    free(cd);
    return 0;
}
