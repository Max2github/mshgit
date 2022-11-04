#include "../include/msh.h"
#include "../include/alg.h"
#include "../include/func.h"
#include "../include/super.h"
#include "../dependencies/words.h"
#include "../dependencies/std.h"

void msh_freeRessources() {
    // execute all functions in MSH_ON_EXIT
    list temp = MSH_ON_EXIT;
    while(temp != NULL) {
        void (*function)() = (void (*)()) temp->el;
        function();
        temp = temp->next;
    }
    s_arr_free(FUNC_SPEICHER);
    s_arr_free(FUNC_NAMES);
    list_free(LIST_SPEICHER); // move to MSH_ON_EXIT later
    list_free(MSH_ON_EXIT);
    FUNC_SPEICHER = NULL;
    FUNC_NAMES = NULL;
    LIST_SPEICHER = NULL; // move to MSH_ON_EXIT later
    MSH_ON_EXIT = NULL;
}

int msh_readScript(char Script[]) {
    replaceS(Script, "\n-)", ";");
    char ** Zeilen;
    int Zeilen_Anzahl = split(Script, "\n", &Zeilen);
    // printf("%d\n", Zeilen_Anzahl);
    msh_info msh = MSH_INFO_DEFAULT;
    for (msh_Script_it = 0; msh_Script_it <= Zeilen_Anzahl; msh_Script_it++) {
        // printf("it: %d\n", msh_Script_it);
        if (!check_Func(Zeilen) && !check_bigdata(&msh, Zeilen)) {
            msh.info.line = msh_Script_it + 1;
            msh_readZeile(&msh, Zeilen[msh_Script_it]);
            if (msh_STOP == 1) {
                msh_STOP = 0;
                break;
            };
        }
    };
    freeWordArr(Zeilen, Zeilen_Anzahl);
    msh_freeRessources();
    return 0;
};
int msh_readFunc(msh_info * msh, const char Script[], const char * funcName) {
    char ** Zeilen;
    int msh_Script_it_echt = msh_Script_it;

    // save in_func and add state in func
    int IN_FUNC_old = IN_FUNC;
    bool msh_in_func_old = msh->info.in_func;
    IN_FUNC = 1;
    msh->info.in_func = true;
    msh_func_deph_add_func(msh, funcName);

    int Zeilen_Anzahl = split(Script, "\n", &Zeilen);
    for (int FuncI = 0; FuncI <= Zeilen_Anzahl; FuncI++) {
        msh->info.line = FuncI + 1;
        msh_readZeile(msh, Zeilen[FuncI]);
        if (msh_STOP == 1) {
            msh_STOP = 0;
            break;
        }
        if (msh_Script_it_echt != msh_Script_it) {
            FuncI = msh_Script_it;
            msh_Script_it = msh_Script_it_echt;
        }
    };
    freeWordArr(Zeilen, Zeilen_Anzahl);
    // reset in_func state

    IN_FUNC = IN_FUNC_old;
    msh->info.in_func = msh_in_func_old;
    msh_func_depth_remove_last_func(msh);

    // reset func-memory / stack
    // msh_func_stack_print(FUNC_STACKS, msh_func_stacks_count(FUNC_STACKS) - 1);
    // FUNC_STACKS = msh_func_remove_last(FUNC_STACKS);
    // if (list_node_len((list) FUNC_STACKS) == 0) { IN_FUNC = 0; }
    // msh_func_remove_last(stack);
    return 0;
};
int msh_readFile(char filename[]) {
    #if FILESYSTEM == 1
        FILE* file;
        file = fopen(filename, "r");
        if(file == NULL) {
            MSH_PRINTF("!! ERROR: Can not open file \"%s\"!\n", filename);
            return -1;
        };
        char * string;
        // go to the end of the file
        fseek(file, 0, SEEK_END);
        int len = ftell(file);
        // go to the beginning of the file
        fseek(file, 0, SEEK_SET);
        string = malloc(len+1 * sizeof(char));
        fread(string, len, 1, file);
        fclose(file);
        // printf("%s\n", string);
        msh_readScript(string);
        free(string);
    #else
        MSH_PRINTF("Error: %s\n", "No filesystem provided!");
    #endif
    return 0;
};