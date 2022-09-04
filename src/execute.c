#include "../include/msh.h"
#include "../include/alg.h"
#include "../include/func.h"
#include "../include/super.h"
#include "../dependencies/words.h"
#include "../dependencies/std.h"

int msh_readScript(char Script[]) {
    replaceS(Script, "\n-)", ";");
    char ** Zeilen;
    int Zeilen_Anzahl = split(Script, "\n", &Zeilen);
    // printf("%d\n", Zeilen_Anzahl);
    for (msh_Script_it = 0; msh_Script_it <= Zeilen_Anzahl; msh_Script_it++) {
        // printf("it: %d\n", msh_Script_it);
        if (!check_Func(Zeilen)) {
            msh_readZeile(Zeilen[msh_Script_it], NULL);
            if (msh_STOP == 1) {
                msh_STOP = 0;
                break;
            };
        }
    };
    freeWordArr(Zeilen, Zeilen_Anzahl);
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
    // msh_func_remove_all();
    return 0;
};
int msh_readFunc(char Script[], FUNC_LOCAL_STACK * stack) {
    char ** Zeilen;
    int msh_Script_it_echt = msh_Script_it;
    IN_FUNC = 1;
    int Zeilen_Anzahl = split(Script, "\n", &Zeilen);
    for (int FuncI = 0; FuncI <= Zeilen_Anzahl; FuncI++) {
        msh_readZeile(Zeilen[FuncI], stack);
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