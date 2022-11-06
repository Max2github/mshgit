#include "../dependencies/std.h"
#include "../include/alg.h"
#include "../dependencies/words.h"

// init func-vars
s_arr FUNC_SPEICHER = NULL;
// /* __thread */ FUNC_STACK FUNC_STACKS = NULL;
s_arr FUNC_NAMES = NULL;

// init global vars
MSH_THREAD_VAR int IN_FUNC = 0;

// Variables & Memory
int VAR_MAXWORDS = 500; // max-ammount of vars
int VAR_MAXCHAR = 4000; // max-length for a line
char VAR_SPEICHER[500][4000]; // Stack / Memory for vars
char VAR_NAMES[500][32]; // saves the names of the vars
int VAR_WORTZAELER = 0; // ammount of vars
int VAR_ZEICHENZAELER = 0; // lenght of the current longest line
list LIST_SPEICHER = NULL; // Stack / Heap / Memory for linked lists (stores in fact only pointers + names)
list MSH_ON_EXIT = NULL;

// needed generally & especially for scripts
MSH_THREAD_VAR char msh_Wert[4000];
int msh_STOP = 0;
int msh_Script_it = 0;

#if MULTI_THREAD
    int MSH_PRINT_MUTEX;
#endif

void msh_error_old(const char * msg) {
    if (IN_FUNC) {
        printf("!! ERROR at line %d (inside a func) : %s!\n", msh_Script_it + 1, msg);
        return;
    }
    printf("!! ERROR at line %d : %s!\n", msh_Script_it + 1, msg);
}

void msh_error(const msh_info * msh, const char * msg) {
    if(msh->info.in_func) {
        printf("!! ERROR at line %d ", msh->info.line);
        msh_func_depth temp = msh->info.funcs;
        SIMPLE_LIST_FOREACH(temp, 
            printf("in %s", temp->data);
        )
        printf(" : %s!\n", msg);
        return;
    }
    printf("!! ERROR at line %d : %s!\n", msh->info.line, msg);
}

void set_msh_Wert_old(const char * w) {
    // while (MSH_MUTEX); // wait till other threads are finished
    // MSH_MUTEX = 1;
    word_copy(msh_Wert, w);
    // MSH_MUTEX = 0;
}

/* const char * get_msh_Wert() {
    while (MSH_MUTEX); // wait till other threads are finished
    return msh_Wert;
} */

void msh_add_on_exit(void(*function)()) {
    MSH_ON_EXIT = list_addFirst(MSH_ON_EXIT, Char_pointer, (char *) function, End);
}

const char * get_msh_Wert_new(msh_info * msh) {
    // return (const char *) info->wert.data;
    return msh->wert;
}
void set_msh_Wert(msh_info * msh, const char * value) {
    // SIMPLE_ARRAY_APPEND_DATA(info->wert, value, word_len(value) + 1);
    // SIMPLE_ARRAY_WRITE(info->wert, 0, value, word_len(value) + 1)
    word_copy(msh->wert, value);
}

void msh_func_deph_add_func(msh_info * msh, const char * name) {
    msh_func_depth temp = msh->info.funcs;
    char * allocValue = MSH_MALLOC(word_len(name) + 1);
    word_copy(allocValue, name);
    SIMPLE_LIST_ADDLAST(temp, allocValue);
    if (msh->info.funcs == NULL) {
        msh->info.funcs = temp;
    }
}

void msh_func_depth_remove_last_func(msh_info * msh) {
    if (msh->info.funcs == NULL) {
        msh_error(msh, "Internal: Could not remove last funcname from info. There are no funcnames in the info!");
        return;
    }
    msh_func_depth temp = msh->info.funcs;
    msh_func_depth before = NULL;
    while ((msh_func_depth) temp->next != NULL) {
        before = temp;
        temp = (msh_func_depth) temp->next;
    }
    msh_func_depth next = (msh_func_depth) temp->next;
    MSH_FREE((char *) temp->data);
    MSH_FREE(temp);
    if (before != NULL) { before->next = (long) next; }
    else { msh->info.funcs = next; }
    return;
}