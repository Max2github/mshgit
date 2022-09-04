#pragma once
#include "../dependencies/std.h"
#include "../include/func.h"
#include "../dependencies/list.h"

#if MULTI_THREAD
    #include <pthread.h>
#endif

#ifndef NULL
    #define NULL 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

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
extern list MSH_ON_EXIT;

// needed generally & especially for scripts
extern MSH_THREAD_VAR char msh_Wert[4000];
extern int msh_STOP;
extern int msh_Script_it;

#if MULTI_THREAD
    extern int MSH_PRINT_MUTEX;
    #if MULTI_THREAD_MUTEX_PRINT
        #define MSH_PUTS(str) { MSH_MUTEX_LOCK(&MSH_PRINT_MUTEX); MSH_PUTS_INTERN(str); MSH_MUTEX_UNLOCK(&MSH_PRINT_MUTEX); }
        #define MSH_PRINTF(...) { MSH_MUTEX_LOCK(&MSH_PRINT_MUTEX); MSH_PRINTF_INTERN(__VA_ARGS__); MSH_MUTEX_UNLOCK(&MSH_PRINT_MUTEX);  }
    #endif
#endif

void msh_error(const char *);
void set_msh_Wert(const char *);
// const char * get_msh_Wert();

void msh_add_on_exit(void(*function)());

#ifdef __cplusplus
}
#endif