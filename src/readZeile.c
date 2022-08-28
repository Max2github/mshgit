#include "../include/alg.h"
#include "../dependencies/std.h"
#include "../dependencies/words.h"
#include "../include/super.h"
#include "../include/msh.h"

#include "cparts/command_def.h"

void msh_command_sub_pointto();

int msh_readZeile(char Zeile[], FUNC_LOCAL_STACK * stack) {
    char ** Zeile_Teile;
    char newZeile[VAR_MAXCHAR];

    int teile_Anzahl = split(Zeile, " ", &Zeile_Teile);
    join((const char **) Zeile_Teile, teile_Anzahl, "", newZeile);
    freeWordArr(Zeile_Teile, teile_Anzahl);

    if (msh_Breaks(newZeile, stack) == 1) {
        return 0;
    };

    if (msh_command_isSpezial(newZeile, stack) != 0) {
        return 0;
    };

    while (IN_FUNC) {
        char altZeile[VAR_MAXCHAR];
        word_copy(altZeile, newZeile);
        msh_fill_local_Var(newZeile, stack);
        if (word_compare(altZeile, newZeile) == 0) {
            break;
        };
    };
    while (1) {
        char altZeile[VAR_MAXCHAR];
        word_copy(altZeile, newZeile);
        msh_fillObj(newZeile);
        msh_fillVar(newZeile);
        if (word_compare(altZeile, newZeile) == 0) {
            break;
        };
    };

    if (msh_Breaks(newZeile, stack) == 1) {
        return 0;
    };

    if (msh_command_isSpezial(newZeile, stack) != 0) {
        return 0;
    };

    char VollZeile[VAR_MAXCHAR];
    word_copy(VollZeile, newZeile);
    replace(newZeile, "_", " ");
    replaceS(newZeile, "&/underscore//", "_");
    // printf("%s\n", newZeile);

    char ** array;
    int arrTeile = split(newZeile, "=", &array);
    if (arrTeile > 0) {
        word_copy(newZeile, array[1]);
    };
    replaceS(newZeile, "&/equals//", "=");
    // word_copy(msh_Wert, newZeile);
    set_msh_Wert(newZeile);
    // printf("%s\n", newZeile);

    // cut functions out of value
    // sub
    #include "cparts/readZeile_commandCut_sub.txt"
    // main
    #include "cparts/readZeile_commandCut_main.txt"


    // pointer- / var- or fill-commands
    int found_pointTo = 0;
    while (1) {
        found_pointTo = find(msh_Wert, "pointTo()");
        if (found_pointTo == 0) {
            break;
        };
        msh_command_sub_pointto();
        // printf("Wert: %s\n", msh_Wert);
    };
    // sub-commands
    #include "cparts/readZeile_commandFunc_sub.txt"

    // main-commands
    #include "cparts/readZeile_commandFunc_main.txt"

    int local_found = 0;
    if (IN_FUNC && arrTeile > 0) {
        local_found = 1;
        // int stack_id = msh_func_stacks_count(FUNC_STACKS) - 1;
        if (find(array[0], ".") != 0) {
            local_found = 0; // not supported yet
        } else {
            char nowhere[VAR_MAXCHAR];
            // if not found as local var, but as global var
            // not found local + found global -> priority: global
            /* if (msh_func_get_local_Var_index(array[0], stack_id) == -1 && msh_get_Var(array[0], nowhere) != -1) {
                local_found = 0;
            } */
            if (msh_func_get_local_Var_index(array[0], stack) == -1 && msh_get_Var(array[0], nowhere) != -1) {
                local_found = 0;
            }
            // not found local + not found global -> priority: local
            // found local + not found global -> priority: local
            // found local + found global -> priority: local
            else {
                msh_func_update_local_Var(array[0], msh_Wert, stack);
                local_found = 1;
            }
        }
    } 
    if (arrTeile > 0 && local_found == 0) {
        if (find(array[0], ".") != 0) {
            // printf("%s = %s\n", array[0], msh_Wert);
            char ** var_el;
            split(array[0], ".", &var_el);
            // printWordArr(var_el, 1);
            char var[VAR_MAXCHAR];
            int index = msh_get_Var(var_el[0], var);
            // printf("    %d : %s\n", index, var);
            if (index == -1) {
                msh_error("Object does not exist!");
                return 1;
            } else {
                char newWert[VAR_MAXCHAR];
                char ** echtEl;
                if (find(var, "&/obj//") != 0) {
                    int echtEl_Teile = split(var, "&/obj//", &echtEl);
                    // printWordArr(echtEl, echtEl_Teile);
                    for (int eEt = 0; eEt <= echtEl_Teile; eEt++) {
                        char ** echtEl_eEt_Teile;
                        int echtEl_eEt_Teile_Anzahl = split(echtEl[eEt], ":", &echtEl_eEt_Teile);
                        int alle = word_compare(echtEl_eEt_Teile[0], var_el[1]);
                        // printf("        %s, %s\n", echtEl_eEt_Teile[0], var_el[1]);
                        freeWordArr(echtEl_eEt_Teile, echtEl_eEt_Teile_Anzahl);
                        if (alle == 0) {
                            int newSize = word_len(var_el[1]) + word_len(msh_Wert) + 2;
                            // printf("    %d\n", newSize);
                            echtEl[eEt] = MSH_REALLOC(echtEl[eEt], newSize * sizeof(char));
                            word_copy(echtEl[eEt], var_el[1]);
                            word_add(echtEl[eEt], ":");
                            word_add(echtEl[eEt], msh_Wert);
                            // sprintf(echtEl[eEt], "%s:%s", var_el[1], msh_Wert);
                            // printf("    %s\n", echtEl[eEt]);
                            break;
                        };
                    };
                    join((const char **) echtEl, echtEl_Teile, "&/obj//", newWert);
                    // printf("   newWert: %s\n", newWert);
                    freeWordArr(echtEl, echtEl_Teile);
                } else if (find(var, "&/arr//") != 0) {
                    int echtEl_Teile = split(var, "&/arr//", &echtEl);
                    for (int eEt = 0; eEt <= echtEl_Teile; eEt++) {
                        int alle = 0;
                        for (int n = 0; n < word_len(var_el[0]); n++) {
                            if (echtEl[eEt][n] == var_el[0][n]) {
                                alle++;
                            } else {
                                alle = 0;
                            };
                        };
                        if (alle == word_len(var_el[0])-1) {
                            word_copy(echtEl[eEt], var_el[1]);
                            // sprintf(echtEl[eEt], "%s", var_el[1]);
                            break;
                        };
                    };
                    join((const char **) echtEl, echtEl_Teile, "&/arr//", newWert);
                    freeWordArr(echtEl, echtEl_Teile);
                } else {
                    return 1;
                };
                word_copy(VAR_SPEICHER[index], newWert);
            };
            freeWordArr(var_el, 1);
            return 0;
        };
        char var[VAR_MAXCHAR];
        int index = msh_get_Var(array[0], var);
        if (index == -1) {
            msh_push_Var(msh_Wert, array[0]);
        } else {
            word_copy(VAR_SPEICHER[index], msh_Wert);
        };
    };
    freeWordArr(array, arrTeile);
    return 0;
};
