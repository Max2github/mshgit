#include "../../../include/msh.h"
#include "../../../include/alg.h"
#include "../../../dependencies/words.h"
#include "../../../dependencies/std.h"

void msh_command_main_display() {
    if (find(msh_Wert, "&/arr//") != 0) {
        replaceS(msh_Wert, "&/arr//", "\n     ");
        printf(">> [\n     %s\n   ]\n", msh_Wert);
    } else if (find(msh_Wert, "&/obj//") != 0) {
        // printf("%s\n", VAR_SPEICHER[0]);
        replaceS(msh_Wert, "&/obj//", "\n     ");
        printf(">> {\n     %s\n   }\n", msh_Wert);
    } else {
        printf(">> %s\n", msh_Wert);
    };
};
void msh_command_main_print() {
    if (find(msh_Wert, "&/arr//") != 0) {
        printf("!! ERROR at line %d : Cannot \"print()\" variable of type \"Array\"!", msh_Script_it + 1);
    } else if (find(msh_Wert, "&/obj//") != 0) {
        printf("!! ERROR at line %d : Cannot \"print()\" variable of type \"object\"!", msh_Script_it + 1);
    } else {
        replaceS(msh_Wert, "\\n", "\n");
        printf("%s", msh_Wert);
    };
};
void msh_command_main_stop() {
    msh_STOP = 1;
};
void msh_command_main_del() {
    char var[VAR_MAXCHAR];
    int index = msh_get_Var(msh_Wert, var);
    // printf("\"%s\"\n", var);
    if (index == -1) {
        char text[] = "Variable &/var// does not exist!";
        replaceS(text, "&/var//", msh_Wert);
        word_copy(msh_Wert, text);
    } else {
        for (int i = index; i < VAR_WORTZAELER-1; i++) {
            word_copy(VAR_NAMES[i], VAR_NAMES[i+1]);
            word_copy(VAR_SPEICHER[i], VAR_SPEICHER[i+1]);
        }
        VAR_NAMES[VAR_WORTZAELER-1][0] = '\0';
        VAR_WORTZAELER--;
    };
};
void msh_command_main_call() {
    msh_func_call(msh_Wert);
    /*
    // getting full name of the function and required arguments
    superstring fullname = msh_get_func_fullname(msh_Wert);
    new_superstring spl = s_init("(");
    new_s_arr nameANDargs = s_split(fullname, spl);
    s_free(spl);
    if (s_arr_len(nameANDargs) > 1) {
        superstring name = s_arr_getEl(nameANDargs, 0);
        superstring args = s_arr_getEl(nameANDargs, 1);
        s_removeChar(args, s_len(args)-1);

        // defining required arguments ("nameless arguments")
        spl = s_init(",");
        new_s_arr argList = s_split(args, spl);
        s_free(spl);
        for (s_arr temp = argList; temp != NULL; temp = temp->next) {
            superstring nameS = temp->element;
            FUNC_VAR_NAMES = s_arr_addFirst(FUNC_VAR_NAMES, s_copy(nameS));
        }
        s_arr_free(argList);
    }
    s_arr_free(nameANDargs);

    // getting and preparing code
    superstring codeS = msh_get_func_code(msh_Wert);
    char codeStr[s_len(codeS) + 1];
    s_stringify(codeS, codeStr);

    // initializing arguments
    if (find(msh_Wert, "(")) {
        char ** wordArr;
        int Teile = split(msh_Wert, "(", &wordArr);
        wordArr[1][word_len(wordArr[1]) - 1] = '\0';

        char ** argArray;
        int argAnzahl = split(wordArr[1], ",", &argArray);
        for (int i = 0; i <= argAnzahl; i++) {
            if (find(argArray[i], ":")) {
                char ** argTeile;
                int argTeile_Anzahl = split(argArray[i], ":", &argTeile);
                if (s_arr_len(FUNC_VAR_SPEICHER) != s_arr_len(FUNC_VAR_NAMES)) {
                    FUNC_VAR_SPEICHER = s_arr_addFirst(FUNC_VAR_SPEICHER, s_init(argTeile[1]));
                } else {
                    msh_update_local_Var(argTeile[0], argTeile[1]);
                }
                freeWordArr(argTeile, argTeile_Anzahl);
            } else if (s_arr_len(FUNC_VAR_SPEICHER) < s_arr_len(FUNC_VAR_NAMES)) {
                FUNC_VAR_SPEICHER = s_arr_addFirst(FUNC_VAR_SPEICHER, s_init(argArray[i]));
            } else {
                msh_error("too many nameless arguments");
            }
        }
        freeWordArr(argArray, argAnzahl);

        freeWordArr(wordArr, Teile);
    }
    if (s_arr_len(FUNC_VAR_SPEICHER) != s_arr_len(FUNC_VAR_NAMES)) {
        // s_arr_print(FUNC_VAR_NAMES);
        // s_arr_print(FUNC_VAR_SPEICHER);
        msh_error("count of required arguments and provided arguments is not even!\n!! Abording!");
        s_arr_free(FUNC_VAR_SPEICHER);
        s_arr_free(FUNC_VAR_NAMES);
        FUNC_VAR_SPEICHER = NULL;
        FUNC_VAR_NAMES = NULL;
        return;
    }
    // executing function
    msh_readFunc(codeStr); */
};
void msh_command_main_goto() {
    msh_Script_it = atoi(msh_Wert)-1;
};