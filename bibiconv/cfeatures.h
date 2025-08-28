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

#if __STDC_VERSION__ >= 201112L && !defined(__cplusplus)
#if __STDC_VERSION__ <= 202311L && !defined(static_assert)
#define static_assert _Static_assert
#endif
#elif !defined(__cplusplus) || !defined(__cpp_static_assert)
#define static_assert(...)
#endif /* __STDC_VERSION__ >= 201112L */

#if (__STDC_VERSION__ >= 202311L) \
    || (defined(__cplusplus) && __cplusplus >= 201103L) \
    || __has_feature(cxx_fixed_enum) || __has_feature(objc_fixed_enum) \
    || __has_feature(cxx_strong_enums)
#define __fixed_enum(type) : type
#else
#define __fixed_enum(type)
#endif

#if (__STDC_VERSION__ < 202311L) \
    && (!defined(__cplusplus) || __cplusplus < 201103L) \
    && !__has_feature(c_nullptr) && !__has_feature(cxx_nullptr)
/**
 * The type of a ``nullptr`` value, representing a "null" pointer.
 *
 * This type was was introduced to C in the C23 standard to make it possible for
 * `_Generic` macros to distinguish a null pointer constant from other pointer
 * values.
 */
typedef void *nullptr_t;
#define nullptr ((nullptr_t)NULL)
#endif

#endif /* BIBICONV_CFEATURES_H */
