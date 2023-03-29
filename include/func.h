#ifndef MSH_FUNC_H
#define MSH_FUNC_H

#include "../dependencies/extern.h"
#include "all_extern.h"

#ifdef __cplusplus
extern "C" {
#endif

struct FUNC_LOCAL_STACK {
    s_arr VAR_SPEICHER;
    s_arr VAR_NAMES;
};
typedef struct FUNC_LOCAL_STACK FUNC_LOCAL_STACK;

// handling vars
int msh_func_get_local_Var_index(const char name[], FUNC_LOCAL_STACK * stack); // get the index of the local Var inside the right stack
superstring msh_func_get_local_Var(const char name[], FUNC_LOCAL_STACK * stack); // get the content of the local Var inside the right stack
void msh_func_set_local_Var(const char name[], const char value[], FUNC_LOCAL_STACK * stack); // set a new local var inside the right stack
void msh_func_update_local_Var(const char name[], const char value[], FUNC_LOCAL_STACK * stack); // update (or set a new, if it doesn't exist) local var inside the right stack

// for getting funcs
superstring msh_func_get_fullname(const char * name); // get the full name of the function. z.B.: name = sayhi, full name = sayhi(arg1, arg2)
int msh_func_get_index(const char name[]); // returns the index of the func (index from FUNC_NAMES)
superstring msh_func_get_code(const char name[]) ; // returns the code / content of the func (index from FUNC_SPEICHER)

// This is basically the main-function in this file
void msh_func_call(msh_info * msh, const char * name); // executes a func (including setting up stack, handling arguments, ...)

#ifdef __cplusplus
}
#endif

#endif