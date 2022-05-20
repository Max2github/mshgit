#include "../include/alg.h"
#include "../dependencies/std.h"
#include "../dependencies/words.h"
#include "../include/super.h"
#include "../include/msh.h"

int msh_readZeile(char Zeile[]) {
    char ** Zeile_Teile;
    char newZeile[VAR_MAXCHAR];

    int teile_Anzahl = split(Zeile, " ", &Zeile_Teile);
    join((const char **) Zeile_Teile, teile_Anzahl, "", newZeile);
    freeWordArr(Zeile_Teile, teile_Anzahl);

    if (msh_Breaks(newZeile) == 1) {
        return 0;
    };

    if (msh_command_isSpezial(newZeile) != 0) {
        return 0;
    };

    while (IN_FUNC) {
        char altZeile[VAR_MAXCHAR];
        word_copy(altZeile, newZeile);
        msh_fill_local_Var(newZeile);
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

    if (msh_Breaks(newZeile) == 1) {
        return 0;
    };

    if (msh_command_isSpezial(newZeile) != 0) {
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
    word_copy(msh_Wert, newZeile);
    // printf("%s\n", newZeile);

    // cut functions out of value
    // sub
    replaceS(msh_Wert, "Array()", "");
    replaceS(msh_Wert, "object()", "");
    replaceS(msh_Wert, "random()", "");
    replaceS(msh_Wert, "randomin()", "");
    replaceS(msh_Wert, "input()", "");
    replaceS(msh_Wert, "math()", "");
    replaceS(msh_Wert, "row()", "");
    replaceS(msh_Wert, "Ascii()", "");
        // list
        replaceS(msh_Wert, "list-toString()", "");
        replaceS(msh_Wert, "list-Len()", "");
        replaceS(msh_Wert, "list-getFirst()", "");
    // main
    replaceS(msh_Wert, "display()", "");
    replaceS(msh_Wert, "print()", "");
    replaceS(msh_Wert, "stop()", "");
    replaceS(msh_Wert, "del()", "");
    replaceS(msh_Wert, "call()", "");
    replaceS(msh_Wert, "goto()", "");
        // list 
        replaceS(msh_Wert, "list-add()", "");
        replaceS(msh_Wert, "list-remove()", "");
        replaceS(msh_Wert, "list-mod()", "");
        replaceS(msh_Wert, "list-Print()", "");
        replaceS(msh_Wert, "list-Del()", "");
        replaceS(msh_Wert, "list-print-SPEICHER()", "");
    // printf("Wert: %s\n", msh_Wert);

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
        // list
        int found_list_toString = find(newZeile, "list-toString()");
        if (found_list_toString != 0) {
            msh_command_sub_list_toString();
        };
        // list
        int found_list_len = find(newZeile, "list-Len()");
        if (found_list_len != 0) {
            msh_command_sub_list_len();
        };
    int found_array = find(newZeile, "Array()");
    if (found_array != 0) {
        msh_command_sub_array();
    };
    int found_object = find(newZeile, "object()");
    if (found_object != 0) {
        msh_command_sub_object();
    };
    int found_random = find(newZeile, "random()");
    if (found_random != 0) {
        msh_command_sub_random();
    };
    int found_randomin = find(newZeile, "randomin()");
    if (found_randomin != 0) {
        msh_command_sub_randomin();
    };
    int found_input = find(newZeile, "input()");
    if (found_input != 0) {
        msh_command_sub_input();
    };
    int found_row = find(newZeile, "row()");
    if (found_row != 0) {
        msh_command_sub_row();
    };
    int found_ascii = find(newZeile, "Ascii()");
    if (found_ascii != 0) {
        msh_command_sub_ascii();
    };
    int found_math = find(newZeile, "math()");
    if (found_math != 0) {
        msh_command_sub_math();
    };
        // list
        int found_list_getFirst = find(newZeile, "list-getFirst()");
        if (found_list_getFirst != 0) {
            msh_command_sub_list_getFirst();
        };

    // main-commands
    int found_display = find(newZeile, "display()");
    if (found_display != 0) {
        msh_command_main_display();
    };
    int found_print = find(newZeile, "print()");
    if (found_print != 0) {
        msh_command_main_print();
    };
    int found_stop = find(newZeile, "stop()");
    if (found_stop != 0) {
        msh_command_main_stop();
    };
    int found_del = find(newZeile, "del()");
    if (found_del != 0) {
        msh_command_main_del();
    };
    int found_call = find(newZeile, "call()");
    if (found_call != 0) {
        msh_command_main_call();
    };
    int found_goto = find(newZeile, "goto()");
    if (found_goto != 0) {
        msh_command_main_goto();
    };
        // list 
        int found_list_add = find(newZeile, "list-add()");
        if (found_list_add != 0) {
            msh_command_main_list_add();
        };
        int found_list_remove = find(newZeile, "list-remove()");
        if (found_list_remove != 0) {
            msh_command_main_list_remove();
        };
        int found_list_mod = find(newZeile, "list-mod()");
        if (found_list_mod != 0) {
            msh_command_main_list_mod();
        };
        int found_list_print = find(newZeile, "list-Print()");
        if (found_list_print != 0) {
            msh_command_main_list_print();
        };
        int found_list_del = find(newZeile, "list-Del()");
        if (found_list_del != 0) {
            msh_command_main_list_delete();
        };
        int found_list_print_SPEICHER = find(newZeile, "list-print-SPEICHER()");
        if (found_list_print_SPEICHER != 0) {
            msh_command_main_list_print_SPEICHER();
        };

    int local_found = 0;
    if (IN_FUNC == 1 && arrTeile > 0) {
        local_found = 1;
        int stack_id = msh_func_stacks_count(FUNC_STACKS) - 1;
        if (find(array[0], ".") != 0) {
            local_found = 0; // not supported yet
        } else {
            char nowhere[VAR_MAXCHAR];
            // if not found as local var, but as global var
            // not found local + found global -> priority: global
            if (msh_func_get_local_Var_index(array[0], stack_id) == -1 && msh_get_Var(array[0], nowhere) != -1) {
                local_found = 0;
            } 
            // not found local + not found global -> priority: local
            // found local + not found global -> priority: local
            // found local + found global -> priority: local
            else {
                msh_func_update_local_Var(array[0], msh_Wert, stack_id);
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
                            echtEl[eEt] = realloc(echtEl[eEt], newSize * sizeof(char));
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
