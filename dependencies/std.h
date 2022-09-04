#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// toogle
#define FILESYSTEM 1
#define MULTI_THREAD 1
#define MULTI_THREAD_MUTEX_PRINT 1 // somehow this doesn't change anything

// DO NOT EDIT
// make sure to set MACROS to 0,
// which are dependent on MACROS set to 0
#if !MULTI_THREAD
    #undef MULTI_THREAD_MUTEX_PRINT
    #define MULTI_THREAD_MUTEX_PRINT 0
#endif

#if FILESYSTEM == 1
    #define MSH_FILE FILE *
    #define MSH_FILE_OPEN fopen
#endif

#if MULTI_THREAD
    #define MSH_THREAD_VAR __thread
    #define MSH_MUTEX_LOCK(mutex) { for(; *mutex == 1; ); *mutex = 1; }
    #define MSH_MUTEX_UNLOCK(mutex) (*mutex = 0)
#else
    #define MSH_THREAD_VAR
#endif

#if MULTI_THREAD_MUTEX_PRINT
    #define MSH_PUTS_INTERN(str) puts(str)
    #define MSH_PRINTF_INTERN(...) printf(__VA_ARGS__)
#else
    #define MSH_PUTS(str) puts(str)
    #define MSH_PRINTF(...) printf(__VA_ARGS__)
#endif

// input
#define MSH_GETCHAR() getchar()
// alloc
#define MSH_MALLOC(size) malloc(size)
#define MSH_REALLOC(oldPtr, newSize) realloc(oldPtr, newSize)
#define MSH_FREE(ptr) free(ptr)
// other
#define MSH_STRING_TO_INT(str) atoi(str)