#include "../dependencies/std.h"
#include "../include/alg.h"

#include "../dependencies/extern.h"

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
// list MSH_ON_EXIT = NULL;

// needed generally & especially for scripts
MSH_THREAD_VAR char msh_Wert[4000];
int msh_STOP = 0;
MSH_THREAD_VAR int msh_Script_it = 0; // if not copied in each thread, a thread can cause the mai program to jump

#if MULTI_THREAD
    // int MSH_PRINT_MUTEX;
    MSH_MUTEX MSH_PRINT_MUTEX = MSH_MUTEX_DEFAULT;
#endif

void msh_error_old(const char * msg) {
    if (IN_FUNC) {
        printf("!! ERROR at line %d (inside a func) : %s!\n", msh_Script_it + 1, msg);
        return;
    }
    printf("!! ERROR at line %d : %s!\n", msh_Script_it + 1, msg);
}

void msh_error(msh_info * msh, const char * msg) {
    if(msh->info.in_func) {
        MSH_MUTEX_LOCK(MSH_PRINT_MUTEX);

        MSH_PRINTF_NO_FLUSH(msh, "!! ERROR at line %d in %s : %s!\n", msh->info.line, msh->info.funcs->data, msg);
        // callstack
        list_print_format(3);
        MSH_PRINTF_NO_FLUSH(msh, "Callstack:\n");
        msh_func_depth temp = msh->info.funcs;
        SIMPLE_LIST_FOREACH(temp,
            list_print_format(5);
            MSH_PRINTF_NO_FLUSH(msh, "%s\n", temp->data);
        )
        msh_flush(msh);

        MSH_MUTEX_UNLOCK(MSH_PRINT_MUTEX);
        return;
    }
    MSH_MUTEX_LOCK(MSH_PRINT_MUTEX);
    MSH_PRINTF(msh, "!! ERROR at line %d : %s!\n", msh->info.line, msg);
    MSH_MUTEX_UNLOCK(MSH_PRINT_MUTEX);
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

void msh_add_on_exit(msh_info * msh, msh_event_callback function) {
    // MSH_ON_EXIT = list_addFirst(MSH_ON_EXIT, Char_pointer, (char *) function, End);
    SIMPLE_LIST_ADDFIRST(msh->event.on.exit, function);
}

char * get_msh_Wert(const msh_info * msh) {
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
    //SIMPLE_LIST_ADDLAST(temp, allocValue);
    SIMPLE_LIST_ADDFIRST(msh->info.funcs, allocValue);
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

void msh_printf(msh_info * msh, const char * format, ...) {
    va_list args;
    va_start(args, format);
    if (msh == NULL) {
        vprintf(format, args);
        va_end(args);
        return;
    }
    /*msh->io.bytesWritten +=*/ vfprintf(msh->io.out, format, args);
    va_end(args);
}

void msh_puts(msh_info * msh, const char * string) {
    /*msh->io.bytesWritten +=*/ fputs(string, msh->io.out);
}

void msh_flush(msh_info * msh) {
    if (msh == NULL) {
        fflush(stdout);
        return;
    }
    fflush(msh->io.out);
    // msh->io.bytesWritten = 0;
}

void msh_io_init(msh_info * msh, bool stdio) {
    // msh->io.bytesWritten = 0;
    msh->io.in = NULL;
    msh->io.out = NULL;
    msh->io.read.out = NULL;
    msh->io.read.in = NULL;

    if (stdio) {
        /*
            msh->io.fd.out.write = STDOUT_FILENO;
            msh->io.fd.out.read = STDOUT_FILENO; // should not be used
            msh->io.fd.in.write = STDIN_FILENO; // should not be used
            msh->io.fd.in.read = STDIN_FILENO; 
        */
        msh->io.out = stdout;
        msh->io.in = stdin; // should not be used
        msh->io.read.out = stdout; // should not be used
        msh->io.read.in = stdin;
        return;
    }

        /*pipe(msh->io.fd.pipe_out);
        msh->io.out = fdopen(msh->io.fd.out.write, "w");
        if (msh->io.out == NULL) {
            msh_error(msh, "Could not open write out pipe.");
        }
        msh->io.read.out = fdopen(msh->io.fd.out.read, "r");
        if (msh->io.out == NULL) {
            msh_error(msh, "Could not open read out pipe.");
        }*/

        // pipe(thread_info.io.fd.pipe_in);
        /*msh->io.in = fdopen(msh->io.fd.in.write, "w");
        if (msh->io.out == NULL) {
            msh_error(msh, "Could not open write in pipe.");
        }
        msh->io.read.in = fdopen(msh->io.fd.in.read, "r");
        if (msh->io.out == NULL) {
            msh_error(msh, "Could not open read in pipe.");
        }*/
}