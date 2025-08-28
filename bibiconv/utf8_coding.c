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

static size_t utf8_read(bib_iconv_t cd, bib_iconv_decoder_t d,
                        char const **restrict src, size_t *restrict srcleft,
                        ucs4_t *unicode) {
    assert(cd != nullptr);
    assert(d != nullptr);
    assert(src != nullptr);
    assert(*src != nullptr);
    assert(srcleft != nullptr);
    assert(unicode != nullptr);

    if (*srcleft == 0) {
        errno = EINVAL;
        return (size_t)-1;
    }

    char8_t first_byte = **src;
    size_t expected_length = 0;
    ucs4_t result = 0;

    if (first_byte < 0x80) {
        expected_length = 1;
        result = first_byte;
    } else if ((first_byte & 0xE0) == 0xC0) {
        expected_length = 2;
        result = first_byte & 0x1F;
    } else if ((first_byte & 0xF0) == 0xE0) {
        expected_length = 3;
        result = first_byte & 0x0F;
    } else if ((first_byte & 0xF8) == 0xF0) {
        expected_length = 4;
        result = first_byte & 0x07;
    } else {
        errno = EILSEQ;
        return (size_t)-1;
    }

    if (*srcleft < expected_length) {
        errno = EINVAL;
        return (size_t)-1;
    }

    for (size_t i = 1; i < expected_length; i++) {
        char8_t byte = (*src)[i];
        if ((byte & 0xC0) != 0x80) {
            errno = EILSEQ;
            return (size_t)-1;
        }
        result = (result << 6) | (byte & 0x3F);
    }

    *unicode = result;
    *src += expected_length;
    *srcleft -= expected_length;
    return expected_length;
}

static size_t utf8_write(bib_iconv_t cd, struct bib_iconv_encoder_s *e,
                         ucs4_t c, char **restrict dst, size_t *restrict len) {
    assert(dst != nullptr);
    assert(*dst != nullptr);
    assert(len != nullptr);
    if (c <= 0x7F) {
        if (*len < 1) {
            errno = E2BIG;
            return (size_t)-1;
        }
        (*dst)[0] = (char8_t)c;
        *dst += 1;
        *len -= 1;
        return 0;
    } else if (c <= 0x7FF) {
        if (*len < 2)  {
            errno = E2BIG;
            return (size_t)-1;
        }
        (*dst)[0] = (char8_t)(0xC0 | (c >> 6));
        (*dst)[1] = (char8_t)(0x80 | (c & 0x3F));
        *dst += 2;
        *len -= 2;
        return 0;
    } else if (c <= 0xFFFF) {
        if (*len < 3)  {
            errno = E2BIG;
            return (size_t)-1;
        }
        (*dst)[0] = (char8_t)(0xE0 | (c >> 12));
        (*dst)[1] = (char8_t)(0x80 | ((c >> 6) & 0x3F));
        (*dst)[2] = (char8_t)(0x80 | (c & 0x3F));
        *dst += 3;
        *len -= 3;
        return 0;
    } else if (c <= 0x10FFFF) {
        if (*len < 4)  {
            errno = E2BIG;
            return (size_t)-1;
        }
        (*dst)[0] = (char8_t)(0xF0 | (c >> 18));
        (*dst)[1] = (char8_t)(0x80 | ((c >> 12) & 0x3F));
        (*dst)[2] = (char8_t)(0x80 | ((c >> 6) & 0x3F));
        (*dst)[3] = (char8_t)(0x80 | (c & 0x3F));
        *dst += 4;
        *len -= 4;
        return 0;
    }
    errno = EINVAL;
    return (size_t)-1;
}

void bib_iconv_open_utf8_encoder(bib_iconv_encoder_t e) {
    e->context = nullptr;
    e->init = nullptr;
    e->write = utf8_write;
    e->flush = nullptr;
    e->deinit = nullptr;
}

void bib_iconv_open_utf8_decoder(bib_iconv_decoder_t d) {
    d->context = nullptr;
    d->init = nullptr;
    d->read = utf8_read;
    d->deinit = nullptr;
}
