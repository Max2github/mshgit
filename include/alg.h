#ifndef MSH_ALG_H
#define MSH_ALG_H

#include "../dependencies/std.h"

#include "func.h"
#include "ref.h"
#include "alg/events.h"
#include "alg/socket.h"

#include "../dependencies/extern.h"
#include "all_extern.h"

#define MSH_IO_SPEICHER_SIZE 4000
#define MSH_IO_DEFAULT { stdout, stdin }
#define MSH_CALLINFO_DEFAULT { 1, false, false, NULL }
#define MSH_EVENTS_ON_DEFAULT { NULL }
#define MSH_EVENTS_DEFAULT { false, false, MSH_EVENTS_ON_DEFAULT }
#define MSH_INFO_DEFAULT { msh_Wert, MSH_CALLINFO_DEFAULT, NULL, NULL, MSH_IO_DEFAULT, MSH_EVENTS_DEFAULT }

#if CC_GCC
    #pragma GCC diagnostic ignored "-Wmissing-braces"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef SIMPLE_LIST(const char *) msh_func_depth;

/**
 * Allow stdin- and -out redirecting for each execution (msh_info).
 */
struct msh_io {
    FILE * out;
    FILE * in;
    // int bytesWritten;
    // index8 speicher[MSH_IO_SPEICHER_SIZE];
    struct {
        FILE * out;
        FILE * in;
    } read;
    struct {
        union {
            int pipe_out[2];
            struct {
                int read;
                int write;
            } out;
        };
        union {
            int pipe_in[2];
            struct {
                int read;
                int write;
            } in;
        };
    } fd;
} ATTRIBUTE_PACKED;
typedef struct msh_io msh_io;

/**
 * Information about location in a script / an execution.
 * 
 */
struct msh_callinfo {
    unsigned int line;
    bool in_func;
    bool in_thread;
    msh_func_depth funcs;
} ATTRIBUTE_PACKED; // 16 bytes (without packed) 2 bytes free
typedef struct msh_callinfo msh_callinfo;

typedef void (*msh_event_callback)();
typedef SIMPLE_LIST(msh_event_callback) msh_event_callback_list;

/**
 * Events for msh_info.
 * Events in "on" should have functions, 
 * which are executed, when an event triggers
 */
struct msh_events {
    bool stop;
    bool exit;
    struct {
        msh_event_callback_list exit;
        // msh_event_callback_list on_init;
    } on;
} ATTRIBUTE_PACKED; // 16 bytes (without packed) 3 bytes free
typedef struct msh_events msh_events;

/**
 * For each code execution there must be a msh_info,
 * which holds information about the execution
 */
struct msh_info {
    char * wert;
    msh_callinfo info;
    FUNC_LOCAL_STACK * stack;
    msh_ref_list refs;
    msh_io io;
    msh_events event;
} ATTRIBUTE_PACKED;

// init func-vars
extern s_arr FUNC_SPEICHER;
// extern /* __thread */ FUNC_STACK FUNC_STACKS;
extern s_arr FUNC_NAMES;

// init global vars

// Variables & Memory
extern int VAR_MAXWORDS; // max-ammount of vars
extern int VAR_MAXCHAR; // max-length for a line
extern char VAR_SPEICHER[500][4000]; // Stack / Memory for vars
extern char VAR_NAMES[500][32]; // saves the names of the vars
extern int VAR_WORTZAELER; // ammount of vars
extern int VAR_ZEICHENZAELER; // lenght of the current longest line
extern list LIST_SPEICHER; // Stack / Heap / Memory for linked lists (stores in fact only pointers + names)

extern msh_events_p_list MSH_EXEC_EVENTS; // set or get events from each execution / registered msh_info

#if MSH_ALLOW_SOCKET
    extern msh_socket_list MSH_SOCKET_ALL;
#endif

// flags
enum msh_flags_bitwise {
    // 1
    msh_flagbit1_none = 0,
    msh_flagbit1_verbose = 1,
};
extern unsigned char MSH_FLAGS_BITWISE1;
#define MSH_VERBOSE_LOG(msh, str) (MSH_FLAGS_BITWISE1 & msh_flagbit1_verbose) ? msh_log(msh, str) : MSH_FLAGS_BITWISE1

// extern list MSH_ON_EXIT;

#define MSH_VAR_NAMES_MAXCHAR sizeof(VAR_NAMES[0])

// needed generally & especially for scripts
extern MSH_THREAD_VAR char msh_Wert[4000];
//extern int msh_STOP;
//extern MSH_THREAD_VAR int msh_Script_it;

#if MSH_ALLOW_MULTI_THREAD
    // mutexe
    extern MSH_MUTEX MSH_PRINT_MUTEX;
    extern MSH_MUTEX MSH_EXEC_EVENTS_MUTEX;
#endif

void msh_error(msh_info *, const char *, ...);
void set_msh_Wert_old(const char *);
void set_msh_Wert(msh_info *, const char *);
char * get_msh_Wert(const msh_info *);

void msh_add_on_exit(msh_info *, msh_event_callback);

void msh_func_deph_add_func(msh_info *, const char *);
void msh_func_depth_remove_last_func(msh_info *);

void msh_printf(msh_info *, const char *, ...);
void msh_vprintf(msh_info *, const char *, va_list);
void msh_putchar(msh_info *, int);
void msh_puts(msh_info *, const char *);
void msh_log(msh_info *, const char *);
void msh_flush(msh_info *);

void msh_io_init(msh_info *, bool);

#ifdef __cplusplus
}
#endif

#endif