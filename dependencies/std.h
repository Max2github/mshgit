#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// print
#define MSH_PUTS(str) puts(str)
#define MSH_PRINTF(...) printf(__VA_ARGS__)
// input
#define MSH_GETCHAR() getchar()
// alloc
#define MSH_MALLOC(size) malloc(size)
#define MSH_REALLOC(oldPtr, newSize) realloc(oldPtr, newSize)
#define MSH_FREE(ptr) free(ptr)
// other
#define MSH_STRING_TO_INT(str) atoi(str)

// toogle
#define FILESYSTEM 1
#define MULTI_THREAD 1

#if FILESYSTEM == 1
    #define MSH_FILE FILE *
    #define MSH_FILE_OPEN fopen
#endif

#if MULTI_THREAD == 1

#endif