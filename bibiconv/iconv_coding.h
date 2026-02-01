//
//  iconv_coding.h
//  bibiconv
//

#ifndef BIB_ICONV_CODING_H
#define BIB_ICONV_CODING_H

#include <stddef.h>
#include <bibiconv/cfeatures.h>
#include <bibiconv/chartypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bib_iconv_s *bib_iconv_t;

#pragma mark - Encoder

/**
 * A collection of functions and state used together to write characters using a
 * particular encoding.
 */
typedef struct bib_iconv_encoder_s {
    /**
     * Arbitrary private data used by the encoder to keep track of state.
     */
    void *context;

    /**
     * Set up ``context`` with initial values at the beginning of conversion.
     *
     * - parameter e: The character encoder to initialize.
     */
    void (*init)(struct bib_iconv_encoder_s *e);

    /**
     * Write a character to the output buffer.
     *
     * - parameter cd: The conversion descriptor.
     * - parameter e: The character encoder.
     * - parameter c: The UCS-4 Unicode code point to write.
     * - parameter dst: The destination buffer to write the converted character
     *   to. After a successful conversion, this buffer is modified to start
     *   after the character written to it.
     * - parameter dstleft: The amount of bytes left in the destination buffer
     *   that converted characters can be written to. After a successful
     *   conversion, this value is set to the length of the buffer after it's
     *   been modified to point after the last converted character written to
     *   it.
     * - returns: `0` on a successful write, and `(size_t)-1` on a failure. A
     *   non-zero positive integer is returned when a non-identical conversion
     *   is written to the destination buffer.
     *
     * ``bib_iconv`` may call this multiple times before calling ``deinit``.
     */
    size_t (*write)(bib_iconv_t cd, struct bib_iconv_encoder_s *e,
                    ucs4_t c, char **restrict dst, size_t *restrict dstleft);

    size_t (*flush)(bib_iconv_t cd, struct bib_iconv_encoder_s *e,
                    char **restrict dst, size_t *restrict dstleft);

    /**
     * Clean up and deallocate the encoder's ``context``.
     *
     * - parameter d: The character encoder to uninitialize.
     */
    void (*deinit)(struct bib_iconv_encoder_s *e);
} *bib_iconv_encoder_t;

/**
 * Sets up an encoder with all the functions necessary to encode UTF-8 encoded
 * text.
 *
 * - parameter e: Allocated space for an encoder.
 */
void bib_iconv_open_utf8_encoder(bib_iconv_encoder_t e);

/**
 * Sets up an encoder with all the functions necessary to encode UTF-16 encoded
 * text.
 *
 * - parameter e: Allocated space for an encoder.
 */
void bib_iconv_open_utf16_encoder(bib_iconv_encoder_t e);

/**
 * Sets up an encoder with all the functions necessary to encode UTF-32 encoded
 * text.
 *
 * - parameter e: Allocated space for an encoder.
 */
void bib_iconv_open_utf32_encoder(bib_iconv_encoder_t e);

#pragma mark - Decoder

/**
 * A collection of functions and state used to read characters from a particular
 * encoding.
 */
typedef struct bib_iconv_decoder_s {
    /**
     * Arbitrary private data used by the decoder to keep track of state.
     */
    void *context;

    /**
     * Set up ``context`` with initial values at the beginning of conversion.
     *
     * - parameter cd: The conversion descriptor.
     * - parameter d: The character decoder to initialize.
     * - parameter src: The source buffer to read encoded data from. After a
     *   successful conversion to UCS-4 Unicode, this buffer is modified to
     *   start after the last character read from the buffer.
     * - parameter srcleft: The amount of bytes left in the input buffer to
     *   read. After a successful conversion to UCS-4 Unicode, this value is
     *   set to the length of the buffer after it's been modified to point after
     *   the last character read.
     * - returns: `0` on successful initialization, and `(size_t)-1` on failure.
     *
     * ``bib_iconv`` may call this multiple times before calling ``deinit``.
     */
    size_t (*init)(bib_iconv_t cd, struct bib_iconv_decoder_s *d,
                   char const **restrict src, size_t *restrict srcleft);

    /**
     * Read a UCS-4 character from the input buffer.
     *
     * - parameter cd: The conversion descriptor.
     * - parameter d: The character decoder.
     * - parameter src: The source buffer to read encoded data from. After a
     *   successful conversion to UCS-4 Unicode, this buffer is modified to
     *   start after the last character read from the buffer.
     * - parameter srcleft: The amount of bytes left in the input buffer to
     *   read. After a successful conversion to UCS-4 Unicode, this value is
     *   set to the length of the buffer after it's been modified to point after
     *   the last character read.
     * - parameter unicode: A pointer to the location to set the converted UCS-4
     *   Unicode code point after a successful conversion.
     * - returns: `0` upon a successful conversion, `(size_t)-1` on failure, and
     *   a positive non-zero integer when encountering an non-identical
     *   conversion to UCS-4 Unicode.
     */
    size_t (*read)(bib_iconv_t cd, struct bib_iconv_decoder_s *d,
                   char const **restrict src, size_t *restrict srcleft,
                   ucs4_t *unicode);

    /**
     * Clean up and deallocate the decoder's ``context``.
     *
     * - parameter d: The character decoder to uninitialize.
     */
    void (*deinit)(struct bib_iconv_decoder_s *d);
} *bib_iconv_decoder_t;

/**
 * Sets up a decoder with all the functions necessary to decode MARC-8 encoded
 * text.
 *
 * - parameter d: Allocated space for a decoder.
 */
void bib_iconv_open_marc8_decoder(bib_iconv_decoder_t d);

/**
 * Sets up a decoder with all the functions necessary to decode UTF-8 encoded
 * text.
 *
 * - parameter d: Allocated space for a decoder.
 */
void bib_iconv_open_utf8_decoder(bib_iconv_decoder_t d);

/**
 * Sets up a decoder with all the functions necessary to decode UTF-16 encoded
 * text.
 *
 * - parameter d: Allocated space for a decoder.
 */
void bib_iconv_open_utf16_decoder(bib_iconv_decoder_t d);

/**
 * Sets up a decoder with all the functions necessary to decode UTF-32 encoded
 * text.
 *
 * - parameter d: Allocated space for a decoder.
 */
void bib_iconv_open_utf32_decoder(bib_iconv_decoder_t d);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BIB_ICONV_CODING_H */
