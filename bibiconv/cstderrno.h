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

#include <errno.h>

#endif /* BIBICONV_CSTDERRNO_H */
