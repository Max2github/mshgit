#include "../dependencies/std.h"

#include "../include/alg.h"
#include "../include/super.h"
#include "../include/msh.h"

#include "../dependencies/extern.h"

#include "cparts/command_def.h"

void msh_command_sub_pointto(msh_info *);

int msh_readZeile(msh_info * msh, const char Zeile[]/*, FUNC_LOCAL_STACK * stack*/) {
    char ** Zeile_Teile;
    char newZeile[VAR_MAXCHAR];

    int teile_Anzahl = split(Zeile, " ", &Zeile_Teile);
    join((const char **) Zeile_Teile, teile_Anzahl, "", newZeile);
    freeWordArr(Zeile_Teile, teile_Anzahl);
    // if (!msh->info.in_func) {
        // MSH_PRINTF(msh, "Zeile: %s\n", newZeile);
    // }

    if (msh_Breaks(msh, newZeile) == 1) {
        return 0;
    };

    if (msh_command_isSpezial(msh, newZeile) != 0) {
        return 0;
    };

    char altZeile[VAR_MAXCHAR];
    while (msh->info.in_func) {
        msh_var_copy_value(msh, altZeile, newZeile);
        msh_fill_local_Var(newZeile, msh->stack);
        if (word_compare(altZeile, newZeile) == 0) {
            break;
        };
    };
    
    while (1) {
        msh_var_copy_value(msh, altZeile, newZeile);
        msh_var_fillObj(msh, newZeile);
        msh_var_fillArr(msh, newZeile);
        msh_var_fillVar(msh, newZeile);
        if (word_compare(altZeile, newZeile) == 0) {
            break;
        };
    };

    if (msh_Breaks(msh, newZeile) == 1) {
        return 0;
    };

    if (msh_command_isSpezial(msh, newZeile) != 0) {
        return 0;
    };

    replace(newZeile, "_", " ");
    replaceS(newZeile, "&/underscore//", "_");
    // printf("%s\n", newZeile);

    // check for equals sign
    char ** array;
    int arrTeile = split(newZeile, "=", &array);
    if (arrTeile > 0) {
        msh_var_copy_value(msh, newZeile, array[1]);
    };
    replaceS(newZeile, "&/equals//", "=");
    // printf("%s\n", newZeile);

    // check for ref append "<<" (only if there is no ref append)
    char ** ref_append_arr = NULL;
    int ref_append_teile = split(newZeile, "<<", &ref_append_arr);
    if (ref_append_teile > 0) {
        msh_var_copy_value(msh, newZeile, ref_append_arr[1]);
    }
    replaceS(newZeile, "&/refappend//", "<<");

    // check for ref assign "<-"
    char ** ref_assign_arr = NULL;
    int ref_assign_teile = split(newZeile, "<-", &ref_assign_arr);
    if (ref_assign_teile > 0) {
        msh_var_copy_value(msh, newZeile, ref_assign_arr[1]);
    }
    replaceS(newZeile, "&/refassign//", "<-");

    set_msh_Wert(msh, newZeile);

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
        msh_command_sub_pointto(msh);
        // printf("Wert: %s\n", msh_Wert);
    };
    // sub-commands
    #include "cparts/readZeile_commandFunc_sub.txt"

    // main-commands
    #include "cparts/readZeile_commandFunc_main.txt"

    // check ref assigning
    if (ref_assign_teile > 0) {
        if (msh_ref_string_is_ref(ref_assign_arr[0])) {
            index32 id = msh_ref_id_from_string(ref_assign_arr[0], word_len(ref_assign_arr[0]));

            index64 len = word_len(msh->wert);

            sString * str = MSH_MALLOC(sizeof(sString));
            *str = sString_init((sString_char_t *) msh->wert, len, SMARTSTRING_FLAG_U_ZERO);

            union msh_ref_data data;
            data.pointer.sstr = str;
            msh_ref_assign(msh, id, data, word_len(msh->wert));
        }
    }
    freeWordArr(ref_assign_arr, ref_assign_teile);

    // check ref appending
    if (ref_append_teile > 0) {
        if (msh_ref_string_is_ref(ref_append_arr[0])) {
            index32 id = msh_ref_id_from_string(ref_append_arr[0], word_len(ref_append_arr[0]));

            index64 len = word_len(msh->wert);

            sString * str = MSH_MALLOC(sizeof(sString));
            *str = sString_init((sString_char_t *) msh->wert, len, SMARTSTRING_FLAG_U_ZERO);

            union msh_ref_data data;
            data.pointer.sstr = str;
            msh_ref_append(msh, id, data, word_len(msh->wert));
        }
    }
    freeWordArr(ref_append_arr, ref_append_teile);

    // check variable assigning

    int local_found = 0;
    if (msh->info.in_func && arrTeile > 0) {
        local_found = 1;
        if (find(array[0], ".") != 0) {
            local_found = 0; // not supported yet
        } else {
            // if not found as local var, but as global var
            // not found local + found global -> priority: global
            if (msh_func_get_local_Var_index(array[0], msh->stack) == -1 && msh_var_getByName(msh, array[0]) != NULL) {
                local_found = 0;
            }
            // not found local + not found global -> priority: local
            // found local + not found global -> priority: local
            // found local + found global -> priority: local
            else {
                msh_func_update_local_Var(array[0], msh_Wert, msh->stack);
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
            int index = msh_var_getIndexByName(msh, var_el[0]);
            const char * var = msh_var_getByIndex(msh, index);
            // printf("    %d : %s\n", index, var);
            if (index == -1) {
                msh_error(msh, "Object does not exist!");
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
                            break;
                        };
                    };
                    join((const char **) echtEl, echtEl_Teile, "&/arr//", newWert);
                    freeWordArr(echtEl, echtEl_Teile);
                } else {
                    return 1;
                };
                msh_var_updateByIndex(msh, newWert, index);
            };
            freeWordArr(var_el, 1);
            return 0;
        };
        int index = msh_var_getIndexByName(msh, array[0]);
        if (index == -1) {
            msh_var_push(msh, get_msh_Wert(msh), array[0]);
        } else {
            msh_var_updateByIndex(msh, get_msh_Wert(msh), index);
        };
    };
    freeWordArr(array, arrTeile);
    return 0;
};
