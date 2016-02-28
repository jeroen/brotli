#include <Rconfig.h>
#ifdef WORDS_BIGENDIAN
#define BROTLI_BUILD_BIG_ENDIAN
#endif

/* some hack for solaris 10 */
#if defined __SUNPRO_CC && !defined __RESTRICT
#include <stdlib.h>
# define _Restrict
# define __restrict
#endif
