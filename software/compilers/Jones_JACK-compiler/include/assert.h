#ifndef ASSERT_H
#define ASSERT_H

#include <stdio.h>
#include <stdint.h>

#define ASSERT(expr, msg) \
    do { \
        if( !( expr ) ) assertFailed(msg, __FILE__, __LINE__); \
    } while(0)

static inline void
assertFailed( const char * msg, const char * file, uint32_t line )
{
    fprintf(stderr, "ERROR: Assert failed in %s at line %d with message: %s\n", file, line, msg);
	exit(-1);
}

#endif // ASSERT_H