//
//  cstderrno.h
//  bibiconv
//

#ifndef BIBICONV_CSTDERRNO_H
#define BIBICONV_CSTDERRNO_H

#ifndef __STDC_LIB_EXT1__
/**
 * Represents the type of a POSIX error code.
 */
typedef int errno_t;
#endif

#ifndef __STDC_WANT_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__ 1
#include <errno.h>
#undef __STDC_WANT_LIB_EXT1__
#else
#include <errno.h>
#endif

#endif /* BIBICONV_CSTDERRNO_H */
