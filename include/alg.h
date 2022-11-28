#ifndef MSH_ALG_H
#define MSH_ALG_H

#include "../dependencies/std.h"

#include "../include/func.h"
#include "../include/ref.h"

#include "../dependencies/extern.h"

#define MSH_IO_SPEICHER_SIZE 4000
#define MSH_IO_DEFAULT { stdout, stdin }
#define MSH_CALLINFO_DEFAULT { 1, false, false, NULL }
#define MSH_EVENTS_ON_DEFAULT { NULL }
#define MSH_EVENTS_DEFAULT { false, MSH_EVENTS_ON_DEFAULT }
#define MSH_INFO_DEFAULT { msh_Wert, MSH_CALLINFO_DEFAULT, NULL, NULL, MSH_IO_DEFAULT, MSH_EVENTS_DEFAULT }

#if CC_GCC
    #pragma GCC diagnostic ignored "-Wmissing-braces"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef SIMPLE_LIST(const char *) msh_func_depth;

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

struct msh_callinfo {
    unsigned int line;
    bool in_func;
    bool in_thread;
    msh_func_depth funcs;
} ATTRIBUTE_PACKED; // 16 bytes (without packed) 2 bytes free
typedef struct msh_callinfo msh_callinfo;

typedef void (*msh_event_callback)();
typedef SIMPLE_LIST(msh_event_callback) msh_event_callback_list;

struct msh_events {
    bool stop;
    struct {
        msh_event_callback_list exit;
        // msh_event_callback_list on_init;
    } on;
} ATTRIBUTE_PACKED; // 16 bytes (without packed) 3 bytes free
typedef struct msh_events msh_events;

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
// extern int MSH_MUTEX;
extern MSH_THREAD_VAR int IN_FUNC;

// Variables & Memory
extern int VAR_MAXWORDS; // max-ammount of vars
extern int VAR_MAXCHAR; // max-length for a line
extern char VAR_SPEICHER[500][4000]; // Stack / Memory for vars
extern char VAR_NAMES[500][32]; // saves the names of the vars
extern int VAR_WORTZAELER; // ammount of vars
extern int VAR_ZEICHENZAELER; // lenght of the current longest line
extern list LIST_SPEICHER; // Stack / Heap / Memory for linked lists (stores in fact only pointers + names)
// extern list MSH_ON_EXIT;

#define MSH_VAR_NAMES_MAXCHAR sizeof(VAR_NAMES[0])

// needed generally & especially for scripts
extern MSH_THREAD_VAR char msh_Wert[4000];
//extern int msh_STOP;
//extern MSH_THREAD_VAR int msh_Script_it;

#if MULTI_THREAD
    extern MSH_MUTEX MSH_PRINT_MUTEX;
#endif

void msh_error_old(const char *);
void msh_error(msh_info *, const char *);
void set_msh_Wert_old(const char *);
// const char * get_msh_Wert();
void set_msh_Wert(msh_info *, const char *);
char * get_msh_Wert(const msh_info *);

void msh_add_on_exit(msh_info *, msh_event_callback);

void msh_func_deph_add_func(msh_info *, const char *);
void msh_func_depth_remove_last_func(msh_info *);

void msh_printf(msh_info *, const char *, ...);
void msh_puts(msh_info *, const char *);
void msh_flush(msh_info *);
void msh_io_init(msh_info *, bool);

#ifdef __cplusplus
}
#endif

#endif