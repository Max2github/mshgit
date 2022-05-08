#pragma once
#include "../dependencies/superstring.h"

// extern s_arr FUNC_SPEICHER; //= NULL; // stores all funcs and their nam
// extern s_arr FUNC_NAMES; // = NULL; // stores the names of the funcs
struct FUNC_ONE_STACK {
    s_arr VAR_SPEICHER;
    s_arr VAR_NAMES;
    struct FUNC_ONE_STACK * next;
};
typedef struct FUNC_ONE_STACK FUNC_ONE_STACK;
typedef FUNC_ONE_STACK * FUNC_STACK;
// extern FUNC_STACK FUNC_STACKS; // = NULL;

// handling stacks
int msh_func_stacks_count(FUNC_STACK head); // how many stacks currently exist
void msh_func_stack_print(FUNC_STACK head, int stack_id); // for debugging
void msh_func_stacks_print(FUNC_STACK head); // for debugging
FUNC_STACK msh_func_create_new_stack(FUNC_STACK head); // create a new stack and append it at the end of the stack-list
FUNC_STACK msh_func_remove_last(FUNC_STACK head); // when the function is finished the latest stack musst be liberted / freed / renoved
void msh_func_remove_all(); // remove alle funcs

// getting the corresponding stack (-parts)
s_arr * msh_func_get_func_var_names_pointer(int func_speicher_index); // get the right var names of the function
s_arr * msh_func_get_func_var_speicher_pointer(int func_speicher_index); // get the right memory / stack of the function 

// for handling local Vars
int msh_func_get_local_Var_index(const char name[], int func_speicher_index); // get the index of the local Var inside the right stack
superstring msh_func_get_local_Var(const char name[], int func_speicher_index); // get the content of the local Var inside the right stack
void msh_func_set_local_Var(const char name[], const char value[], int func_speicher_index); // set a new local var inside the right stack
void msh_func_update_local_Var(const char name[], const char value[], int func_speicher_index); // update (or set a new, if it doesn't exist) local var inside the right stack

// for getting funcs
superstring msh_func_get_fullname(const char * name); // get the full name of the function. z.B.: name = sayhi, full name = sayhi(arg1, arg2)
int msh_func_get_index(const char name[]); // returns the index of the func (index from FUNC_NAMES)
superstring msh_func_get_code(const char name[]) ; // returns the code / content of the func (index from FUNC_SPEICHER)

// This is basically the main-function in this file
void msh_func_call(const char * name); // executes a func (including setting up stack, handling arguments, ...)