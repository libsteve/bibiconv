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

static size_t utf32_read(bib_iconv_t cd, bib_iconv_decoder_t d,
                         char const **restrict src, size_t *restrict srcleft,
                         ucs4_t *unicode) {
    assert(cd != nullptr);
    assert(d != nullptr);
    assert(src != nullptr);
    assert(*src != nullptr);
    assert(srcleft != nullptr);
    assert(unicode != nullptr);

    if (*srcleft < 4) {
        errno = EINVAL;
        return (size_t)-1;
    }

    char32_t code_point = *(char32_t const *)src;
    if (code_point > 0x10FFFF
        || (code_point >= 0xD800 && code_point <= 0xDFFF)) {
        errno = EILSEQ;
        return (size_t)-1;
    }

    *unicode = code_point;
    static_assert(sizeof(*unicode) == sizeof(char32_t),
                  "ucs4_t and char32_t must be the same size.");

    *src += sizeof(char32_t);
    *srcleft -= sizeof(char32_t);
    return 0;
}

static size_t utf32_write(bib_iconv_t cd, struct bib_iconv_encoder_s *e,
                          ucs4_t c, char **restrict dst, size_t *restrict len) {
    assert(cd != nullptr);
    assert(e != nullptr);
    assert(dst != nullptr);
    assert(*dst != nullptr);
    assert(len != nullptr);

    if (*len < 4) {
        errno = E2BIG;
        return (size_t)-1;
    }

    if (c > 0x10FFFF || (c >= 0xD800 && c <= 0xDFFF)) {
        errno = EILSEQ;
        return (size_t)-1;
    }

    *(char32_t *)(*dst) = (char32_t)c;
    static_assert(sizeof(c) == sizeof(char32_t),
                  "ucs4_t and char32_t must be the same size.");

    *dst += sizeof(char32_t);
    *len -= sizeof(char32_t);
    return 0;
}


void bib_iconv_open_utf32_encoder(bib_iconv_encoder_t e) {
    e->context = nullptr;
    e->init = nullptr;
    e->write = utf32_write;
    e->flush = nullptr;
    e->deinit = nullptr;
}

void bib_iconv_open_utf32_decoder(bib_iconv_decoder_t d) {
    d->context = nullptr;
    d->init = nullptr;
    d->read = utf32_read;
    d->deinit = nullptr;
}
