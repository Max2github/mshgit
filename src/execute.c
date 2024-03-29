#include "../include/msh.h"
#include "../include/alg.h"
#include "../include/func.h"
#include "../include/super.h"
#include "../dependencies/std.h"

#include "../dependencies/extern.h"

void msh_exec_init(msh_info * msh, bool use_std_IO) {
    msh_io_init(msh, use_std_IO); // true for use stdio
    msh_exec_event_register(msh);
    msh_val_init(msh);
}

void msh_exec_free(msh_info * msh) {
    msh_exec_event_unregister(msh);
    // execute all functions in MSH_ON_EXIT
    msh_event_callback_list temp = msh->event.on.exit;
    SIMPLE_LIST_FOREACH(temp,
        temp->data();
    )
    SIMPLE_LIST_FREE(msh->event.on.exit);
    msh->event.on.exit = NULL;

    // free callstack
    SIMPLE_LIST_FREE(msh->info.funcs);
    msh->info.funcs = NULL;

    // free all refs
    msh_ref_freeAll(msh);
    msh_val_free(msh);
    msh->refs = NULL;
}

void msh_freeRessources() {
    s_arr_free(FUNC_SPEICHER);
    s_arr_free(FUNC_NAMES);
    list_free(LIST_SPEICHER); // move to MSH_ON_EXIT later
    FUNC_SPEICHER = NULL;
    FUNC_NAMES = NULL;
    LIST_SPEICHER = NULL; // move to MSH_ON_EXIT later
}

int msh_readScript(char Script[]) {
    replaceS(Script, "\n-)", ";");
    char ** Zeilen;
    int Zeilen_Anzahl = split(Script, "\n", &Zeilen);
    // printf("%d\n", Zeilen_Anzahl);
    msh_info msh = MSH_INFO_DEFAULT;
    msh_exec_init(&msh, true);
    
    for (int msh_Script_it = 0; msh_Script_it <= Zeilen_Anzahl; msh_Script_it++) {
        msh.info.line = msh_Script_it + 1;
        if (!check_Func(&msh, Zeilen) && !check_bigdata(&msh, Zeilen)) {
            msh_readZeile(&msh, Zeilen[msh_Script_it]);
            if (msh.event.exit) { break; }
            if (msh.event.stop) {
                msh.event.stop = false;
                break;
            }
        }
        msh_Script_it = msh.info.line - 1;
    };
    freeWordArr(Zeilen, Zeilen_Anzahl);
    msh_exec_free(&msh);
    msh_freeRessources();
    return 0;
};
int msh_readFunc(msh_info * msh, const char Script[], const char * funcName) {
    char ** Zeilen;
    // int msh_Script_it_echt = msh_Script_it;

    // save in_func and add state in func
    bool msh_in_func_old = msh->info.in_func;
    msh_func_deph_add_func(msh, funcName);
    msh->info.in_func = true;
    // save line
    unsigned int msh_info_line_old = msh->info.line;

    int Zeilen_Anzahl = split(Script, "\n", &Zeilen);
    for (int FuncI = 0; FuncI <= Zeilen_Anzahl; FuncI++) {
        msh->info.line = FuncI + 1;
        msh_readZeile(msh, Zeilen[FuncI]);
        if (msh->event.exit) { break; }
        if (msh->event.stop) {
            msh->event.stop = false;
            break;
        }
        FuncI = msh->info.line - 1;
    };
    freeWordArr(Zeilen, Zeilen_Anzahl);

    // reset in_func state
    msh->info.in_func = msh_in_func_old;
    msh_func_depth_remove_last_func(msh);
    // reset line
    msh->info.line = msh_info_line_old;

    // reset func-memory / stack
    // msh_func_stack_print(FUNC_STACKS, msh_func_stacks_count(FUNC_STACKS) - 1);
    // FUNC_STACKS = msh_func_remove_last(FUNC_STACKS);
    // if (list_node_len((list) FUNC_STACKS) == 0) { IN_FUNC = 0; }
    // msh_func_remove_last(stack);
    return 0;
};
int msh_readFile(char filename[]) {
    #if MSH_ALLOW_FILESYSTEM
        FILE* file;
        file = fopen(filename, "r");
        if(file == NULL) {
            MSH_PRINTF(NULL, "!! ERROR: Can not open file \"%s\"!\n", filename);
            return -1;
        };
        char * string;
        // go to the end of the file
        fseek(file, 0, SEEK_END);
        int len = ftell(file);
        // go to the beginning of the file
        fseek(file, 0, SEEK_SET);
        string = MSH_MALLOC(len+1 * sizeof(char));
        fread(string, len, 1, file);
        fclose(file);
        // printf("%s\n", string);
        msh_readScript(string);
        MSH_FREE(string);
    #else
        MSH_PRINTF(NULL, "Error: %s\n", "No filesystem provided!");
    #endif
    return 0;
};