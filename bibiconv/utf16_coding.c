//
//  utf8_coding.c
//  bibiconv
//

#include "cfeatures.h"
#include "chartypes.h"
#include "iconv_coding.h"
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>

static size_t utf16_read(bib_iconv_t cd, bib_iconv_decoder_t d,
                         char const **restrict src, size_t *restrict srcleft,
                         ucs4_t *unicode) {
    assert(cd != nullptr);
    assert(d != nullptr);
    assert(src != nullptr);
    assert(*src != nullptr);
    assert(srcleft != nullptr);
    assert(unicode != nullptr);

    if (*srcleft < 2) {
        errno = EINVAL;
        return (size_t)-1;
    }

    char16_t first_unit = ((char8_t)(*src)[0]) | ((char8_t)(*src)[1] << 8);

    if (first_unit < 0xD800 || first_unit > 0xDFFF) {
        *unicode = first_unit;
        *src += sizeof(char16_t);
        *srcleft -= sizeof(char16_t);
        return 0;
    }

    if (first_unit >= 0xD800 && first_unit <= 0xDBFF) {
        if (*srcleft < 4) {
            errno = EINVAL;
            return (size_t)-1;
        }
        char16_t second_unit = ((unsigned char)(*src)[2]
                                | (unsigned char)(*src)[3] << 8);
        if (second_unit < 0xDC00 || second_unit > 0xDFFF) {
            errno = EILSEQ;
            return (size_t)-1;
        }
        *unicode = ((first_unit - 0xD800) << 10)
                 + (second_unit - 0xDC00)
                 + 0x10000;
        *src += sizeof(char16_t) * 2;
        *srcleft -= sizeof(char16_t) * 2;
        return 0;
    }

    errno = EILSEQ;
    return (size_t)-1;
}

static size_t utf16_write(bib_iconv_t cd, struct bib_iconv_encoder_s *e,
                          ucs4_t c, char **restrict dst, size_t *restrict len) {
    assert(dst != nullptr);
    assert(*dst != nullptr);
    assert(len != nullptr);

    if (c <= 0xFFFF) {
        if (*len < 2) {
            errno = E2BIG;
            return (size_t)-1;
        }
        ((char16_t *)(*dst))[0] = (char16_t)c;
        *dst += 2;
        *len -= 2;
        return 0;
    } else if (c <= 0x10FFFF) {
        if (*len < 4) {
            errno = E2BIG;
            return (size_t)-1;
        }
        c -= 0x10000;
        ((char16_t *)(*dst))[0] = (char16_t)(0xD800 | (c >> 10));
        ((char16_t *)(*dst))[1] = (char16_t)(0xDC00 | (c & 0x3FF));
        *dst += 4;
        *len -= 4;
        return 0;
    }
    errno = EINVAL;
    return (size_t)-1;
}

void bib_iconv_open_utf16_encoder(bib_iconv_encoder_t e) {
    e->context = nullptr;
    e->init = nullptr;
    e->write = utf16_write;
    e->flush = nullptr;
    e->deinit = nullptr;
}

void bib_iconv_open_utf16_decoder(bib_iconv_decoder_t d) {
    d->context = nullptr;
    d->init = nullptr;
    d->read = utf16_read;
    d->deinit = nullptr;
}
