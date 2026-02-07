//
//  cfeatures.h
//  bibiconv
//

#ifndef BIBICONV_CFEATURES_H
#define BIBICONV_CFEATURES_H

#ifndef __has_feature
#define __has_feature(feature) 0
#endif

#ifndef __has_extension
#define __has_extension(extension) 0
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define BIBICONV_HAS_STATIC_ASSERT 1
#if __STDC_VERSION__ < 202311L && !defined(static_assert)
#define static_assert _Static_assert
#endif
#elif defined(__cplusplus) && defined(__cpp_static_assert)
#define BIBICONV_HAS_STATIC_ASSERT 1
#else
#define BIBICONV_HAS_STATIC_ASSERT 0
#define static_assert(...)
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */

#if (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L) \
    && (!defined(__cplusplus) || __cplusplus < 201103L) \
    && !__has_feature(c_nullptr) && !__has_feature(cxx_nullptr) \
    && (!defined(__GNUC__) || __GNUC__ < 13 || __STDC_VERSION__ < 202000L)
/**
 * The type of a ``nullptr`` value, representing a "null" pointer.
 *
 * This type was was introduced to C in the C23 standard to make it possible for
 * `_Generic` macros to distinguish a null pointer constant from other pointer
 * values.
 */
typedef void *nullptr_t;
#define nullptr ((nullptr_t)NULL)
#define BIBICONV_HAS_NULLPTR 0
#else
#define BIBICONV_HAS_NULLPTR 1
#endif /* (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L) \
    && (!defined(__cplusplus) || __cplusplus < 201103L) \
    && !__has_feature(c_nullptr) && !__has_feature(cxx_nullptr) \
    && (!defined(__GNUC__) || __GNUC__ < 13 || __STDC_VERSION__ < 202000L) */

#endif /* BIBICONV_CFEATURES_H */
