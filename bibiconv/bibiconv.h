//
//  bibiconv.h
//  bibiconv
//

#ifndef BIBICONV_H
#define BIBICONV_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#ifndef restrict
#define __BIB_DEFINED_RESTRICT
#if defined(__GNUC__) || defined(__clang__)
#define restrict __restrict__
#elif defined(_MSC_VER)
#define restrict __restrict
#else
#define restrict
#endif
#endif
#endif

//! Project version number for bibiconv.
extern double bibiconvVersionNumber;

//! Project version string for bibiconv.
extern const unsigned char bibiconvVersionString[];

/**
 * A descriptor handle used for converting text between different character
 * encodings.
 */
typedef struct bib_iconv_s *bib_iconv_t;

/**
 * Creates a descriptor used to convert from one character encoding to another.
 *
 * - parameter to: The character encoding to convert text to.
 * - parameter from: The character encoding to convert text from.
 * - returns: A conversion descriptor to use when converting text between
 *   character encodings. Returns `(bib_iconv_t)-1` when there's an error
 *   opening a descriptor.
 *
 * ## Errors
 *
 * - term **`EINVAL`**: Either `to` or `from` don't describe a valid character
 *   encoding supported by the bibiconv library.
 * - term **`ENOMEM`**: The function ran out of heap memory when attempting to
 *   allocate space for the descriptor and its internal data structures.
 *
 * ## Discussion
 *
 * bibiconv supports converting from MARC-8 to UTF-8, UTF-16, and UTF-32, and
 * can convert between the different Unicode formats, but cannot yet convert
 * from Unicode to MARC-8.
 *
 * Valid character encoding values include: "MARC-8", "UTF-8", "UTF-16",
 * "UTF-32", and variations of those values as lowercase and without the dash.
 */
bib_iconv_t bib_iconv_open(char const *restrict to, char const *restrict from);

/**
 * Cleans up and destroys a conversion descriptor.
 *
 * - parameter cd: The conversion descriptor to close.
 * - returns: `0` when the conversion descriptor is closed and deallocated.
 */
int bib_iconv_close(bib_iconv_t cd);

/**
 * Converts text in a given buffer from one encoding to another.
 *
 * - parameter cd: The conversion descriptor used to convert the given string.
 * - parameter src: The source string to convert. After a successful conversion,
 *   the pointer is modified to start after the last converted character. Pass
 *   `NULL` to "flush" the conversion descriptor of any pending characters it
 *   still needs to write to the destination buffer.
 * - parameter srcleft: The amount of bytes left in the given source string that
 *   need to be converted. After a successful conversion, this value is set to
 *   the length of the string after its been modified to start after the last
 *   converted character.
 * - parameter dst: The destination buffer to write the converted string to.
 *   After a successful conversion, this pointer is modified to start after the
 *   last character written to it.
 * - parameter dstleft: The amount of bytes left in the destination buffer that
 *   converted characters can be written to. After a successful conversion, this
 *   value is set to the length of the buffer after it's been modified to point
 *   after the last converted character written to it.
 * - returns: `0` upon a successful conversion, or `(size_t)-1` on error. A
 *   positive non-zero value identifies the number of non-identical conversions
 *   to the destination character encoding—i.e. the conversion was successful,
 *   but some characters couldn't be converted properly.
 *
 * ## Errors
 *
 * When this function returns `(size_t)-1`, `errno` is set to a value
 * identifying the reason for the conversion failure.
 *
 * - term **`EINVAL`**: Either `cd`, `dst`, `*dst`, or `dstleft` are `NULL`.
 * - term **`E2BIG`**: The conversion ran out of space in the destination
 *   buffer. Allocate more space for the destination, then call `bib_iconv`
 *   again with the new values of `src` and `srcleft` to continue the
 *   conversion.
 * - term **`EILSEQ`**: An invalid character sequence was encountered in the
 *   source buffer. Handle the invalid character sequence by correcting it or
 *   skipping over it, then call `bib_iconv` again with the new values of `src`
 *   and `srcleft` to continue the conversion.
 * - term **`ENOMEM`**: The function ran out of heap memory when attempting to
 *   allocate space for its internal data structures.
 *
 * ## Discussion
 *
 * This function works much like the POSIX `iconv` function for converting
 * between character encodings. It provides a specialized implementation for
 * converting from MARC-8 encoded text to UTF-8, UTF-16, and UTF-32.
 *
 * When an invalid multibyte sequence is encountered, or if there is not enough
 * room in the destination buffer, the conversion is stopped and the pointers
 * and size values for `src` and `dst` are updated to reflect the conversion
 * progress. The function will then return `(size_t)-1` and set `errno` to
 * `EILSEQ` for the invalid sequence, or `E2BIG` for running out of space.
 *
 * To finalize conversion, always make sure to call `bib_iconv` with a `NULL`
 * `src` and `srcleft` to flush out any remaining state from the conversion
 * descriptor. This allows the conversion descriptor to write any pending
 * characters to the destination buffer, which may include shift sequences
 * necessary in stateful encodings like MARC-8.
 *
 * ## Example
 *
 * ```c
 * // MARC-8 encoding for "Café"
 * char const source[] = "Caf\xE2\x65";
 * char const *src = source;
 * size_t srcleft = strlen(input);
 *
 * // Destination buffer for UTF-8
 * char destination[256] = { 0 };
 * char *dst = destination;
 * size_t dstleft = sizeof(destination);
 *
 * // Open a conversion descriptor (MARC-8 -> UTF-8)
 * bib_iconv_t cd = bib_iconv_open("UTF-8", "MARC-8");
 * if (cd == (bib_iconv_t)-1) {
 *     perror("bib_iconv_open");
 *     return;
 * }
 *
 * // Convert the string to UTF-8
 * size_t result = bib_iconv(cd, &src, &srcleft, &dst, &dstleft);
 * if (result == (size_t)-1) {
 *     perror("bib_iconv");
 *     return;
 * }
 *
 * // Flush any pending state from the descriptor
 * result = bib_iconv(cd, nullptr, nullptr, &dst, &dstleft);
 * if (result == (size_t)-1) {
 *     perror("bib_iconv flush");
 *     return;
 * }
 *
 * // Close the conversion descriptor
 * bib_iconv_close(cd);
 *
 * // Null-terminate the resulting string
 * *dst = '\0';
 *
 * // Print the result
 * printf("%s", destination);
 * ```
 */
size_t bib_iconv(bib_iconv_t cd,
                 char const **restrict src, size_t *restrict srcleft,
                 char **restrict dst, size_t *restrict dstleft);

#ifdef __cplusplus
#ifdef __BIB_DEFINED_RESTRICT
#undef restrict
#endif
} /* extern "C" */
#endif

#endif /* BIBICONV_H */
