#ifndef MSH_DEP_STD_H
#define MSH_DEP_STD_H

#include <stdarg.h>

#ifndef NO_STD_LIB
    #include <stdio.h>
    #include <stdlib.h>

    // input
    #define MSH_GETCHAR() getchar()
    // alloc
    #define MSH_MALLOC(size) malloc(size)
    #define MSH_REALLOC(oldPtr, newSize) realloc(oldPtr, newSize)
    #define MSH_FREE(ptr) free(ptr)
    // other
    #define MSH_STRING_TO_INT(str) atoi(str)
#else

#endif

#undef SIMPLE_ARRAY_H_MALLOC
#undef SIMPLE_ARRAY_H_REALLOC
#undef SIMPLE_ARRAY_H_FREE

#undef SIMPLE_LIST_H_MALLOC
#undef SIMPLE_LIST_H_FREE

#define SIMPLE_ARRAY_H_MALLOC  MSH_MALLOC
#define SIMPLE_ARRAY_H_REALLOC MSH_REALLOC
#define SIMPLE_ARRAY_H_FREE    MSH_FREE

#define SIMPLE_LIST_H_MALLOC  MSH_MALLOC
#define SIMPLE_LIST_H_FREE    MSH_FREE

#include "../../../lib/templates/def.h"
#include "../../../lib/templates/simple_list.h"
#include "../../../lib/templates/simple_array.h"

//#undef NULL
//#define NULL ((void *) 0)

#if OS_UNIX
    #define MSH_SLEEP(ms) sleep(ms)
#elif OS_WINDOWS
    #define MSH_SLEEP(ms)
#endif

// YOU CAN TOGGLE THESE (0 or 1, 0 -> unset, 1 -> set)
#define MSH_ALLOW_FILESYSTEM 1
#define MSH_ALLOW_MULTI_THREAD 0
#define MSH_ALLOW_SOCKET 1

// DO NOT EDIT

#if MSH_ALLOW_FILESYSTEM
    #define MSH_FILE FILE *
    #define MSH_FILE_OPEN fopen
#endif

#if MSH_ALLOW_SOCKET
    #if OS_UNIX
        #include <netinet/in.h>
        #include <sys/socket.h>
        #include <arpa/inet.h>

        #define MSH_SOCKET_ALL_INIT
        #define MSH_SOCKET_ALL_CLEANUP

        #define MSH_SOCKET int
        #define MSH_SOCKET_DEFAULT 

    #elif OS_WINDOWS
        #include <winsock2.h>
        #include <ws2def.h>
        #include <ws2tcpip.h>
        #include <windows.h>
        // don't need them, everything is in windows.h
        //#include <winsock2.h>
        //#include <ws2tcpip.h>
        //#include <iphlpapi.h>

        // tell linker to include this static library - only for VS, compiler: MSC (CL) / ClangCl
        #if CC_MSC
            #pragma comment(lib, "Ws2_32.lib")
        #endif

        #define MSH_SOCKET_ALL_INIT { \
            WSADATA wsaData; \
            int err = WSAStartup(MAKEWORD(2,2), &wsaData); \
            if (err != 0) { msh_error(NULL, "WSAStartup failed with an error!"); } \
        }
        #define MSH_SOCKET_ALL_CLEANUP WSACleanup() 

        #define MSH_SOCKET SOCKET
        #define MSH_SOCKET_DEFAULT INVALID_SOCKET

    #endif
#endif

#if MSH_ALLOW_MULTI_THREAD
    #define MSH_THREAD_VAR __thread
    #if OS_WINDOWS && !OS_POSIX
        #include <windows.h>
        // threads
        #define MSH_THREAD HANDLE
        #define MSH_THREAD_ID DWORD
        #define MSH_THREAD_CREATE(thread, threadID, func, args, stacksize) { \
            thread = CreateThread( \
                /*security*/ NULL, \
                /*stacksize*/ stacksize, \
                (LPTHREAD_START_ROUTINE) &(func), \
                /*args*/ (void *) &(args), \
                /*flags*/ 0, \
                /*Thread ID*/ &threadID, \
            ); }
        // mutexes
        #define MSH_MUTEX HANDLE
        #define MSH_MUTEX_DEFAULT CreateMutex(NULL, FALSE, NULL);
        #define MSH_MUTEX_LOCK(mutex) WaitForSingleObject(mutex, INFINITE)
        #define MSH_MUTEX_UNLOCK(mutex) ReleaseMutex(mutex)
    #else
        #include <pthread.h>
        // threads
        #define MSH_THREAD pthread_t
        #define MSH_THREAD_ID pthread_t
        #define MSH_THREAD_CREATE(thread, threadID, func, args, stacksize) \
            pthread_create( \
                /*thread/threadID*/ &(thread), \
                /*attributes*/ NULL, \
                (void *) &(func), \
                /*args*/ (void *) &(args) \
            );
        // mutexes
        #define MSH_MUTEX pthread_mutex_t
        #define MSH_MUTEX_DEFAULT PTHREAD_MUTEX_INITIALIZER
        #define MSH_MUTEX_LOCK(mutex) pthread_mutex_lock(&mutex)
        #define MSH_MUTEX_UNLOCK(mutex) pthread_mutex_unlock(&mutex)
    #endif
#else
    #define MSH_THREAD_VAR

    // mutexes
    #define MSH_MUTEX char
    #define MSH_MUTEX_DEFAULT 0
    #define MSH_MUTEX_LOCK(mutex) 
    #define MSH_MUTEX_UNLOCK(mutex) 
#endif

#define MSH_PUTCHAR_NO_FLUSH(msh, c) msh_putchar(msh, c)
#define MSH_PUTS_NO_FLUSH(msh, ...) msh_puts(msh, __VA_ARGS__)
#define MSH_PRINTF_NO_FLUSH(msh, ...) msh_printf(msh, __VA_ARGS__)
#define MSH_VPRINTF_NO_FLUSH(msh, format, args) msh_vprintf(msh, format, args)

#define MSH_PUTCHAR(msh, c) MSH_PUTCHAR_NO_FLUSH(msh, c); msh_flush(msh)
#define MSH_PUTS(msh, str) MSH_PUTS_NO_FLUSH(msh, str); msh_flush(msh)
#define MSH_PRINTF(msh, ...) MSH_PRINTF_NO_FLUSH(msh, __VA_ARGS__); msh_flush(msh)
#define MSH_VPRINTF(msh, format, args) MSH_VPRINTF_NO_FLUSH(msh, format, args); msh_flush(msh)

#endif
