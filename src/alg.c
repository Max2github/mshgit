#include "../include/alg.h"
#include "../dependencies/std.h"
#include "../dependencies/words.h"

// init func-vars
s_arr FUNC_SPEICHER = NULL;
/* __thread */ FUNC_STACK FUNC_STACKS = NULL;
s_arr FUNC_NAMES = NULL;

// init global vars
__thread int IN_FUNC = 0;

// Variables & Memoryxw
int VAR_MAXWORDS = 500; // max-ammount of vars
int VAR_MAXCHAR = 4000; // max-length for a line
char VAR_SPEICHER[500][4000]; // Stack / Memory for vars
char VAR_NAMES[500][32]; // saves the names of the vars
int VAR_WORTZAELER = 0; // ammount of vars
int VAR_ZEICHENZAELER = 0; // lenght of the current longest line
list LIST_SPEICHER = NULL; // Stack / Heap / Memory for linked lists (stores in fact only pointers + names)

// needed generally & especially for scripts
__thread char msh_Wert[4000];
int msh_STOP = 0;
int msh_Script_it = 0;

void msh_error(const char * msg) {
    if (IN_FUNC) {
        printf("!! ERROR at line %d (inside a func) : %s!\n", msh_Script_it + 1, msg);
        return;
    }
    printf("!! ERROR at line %d : %s!\n", msh_Script_it + 1, msg);
}

void set_msh_Wert(const char * w) {
    // while (MSH_MUTEX); // wait till other threads are finished
    // MSH_MUTEX = 1;
    word_copy(msh_Wert, w);
    // MSH_MUTEX = 0;
}

/* const char * get_msh_Wert() {
    while (MSH_MUTEX); // wait till other threads are finished
    return msh_Wert;
} */