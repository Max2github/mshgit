#include "../../dependencies/std.h"

#include "../../include/alg.h"
#include "../../include/msh.h"
#include "../../include/super.h"

#include "../../dependencies/extern.h"

int msh_command_isSpezial(msh_info * msh, char Code[]) {
    if (find(Code, ":") == 0) {
        return 0;
    };
    char ** Code_Teile;
    int CodeTeile = split(Code, ":", &Code_Teile);
    char newText[VAR_MAXCHAR];
    char repl[VAR_MAXCHAR];
    join((const char **) Code_Teile, CodeTeile, ":", newText);
    // sprintf(repl, "%s:", Code_Teile[0]);
    word_copy(repl, Code_Teile[0]);
    word_add(repl, ":");
    replaceS(newText, repl, "");
    char newCond[VAR_MAXCHAR];
    word_copy(newCond, Code_Teile[0]);
    freeWordArr(Code_Teile, CodeTeile);

    while (IN_FUNC) {
        char altCond[VAR_MAXCHAR];
        word_copy(altCond, newCond);
        msh_fill_local_Var(newCond, msh->stack);
        if (word_compare(altCond, newCond) == 0) {
            break;
        };
    };
    while (1) {
        char altCond[VAR_MAXCHAR];
        word_copy(altCond, newCond);
        // msh_fillObj(newCond);
        // msh_fillVar(newCond);
        msh_var_fillObj(msh, newCond);
        msh_var_fillArr(msh, newCond);
        msh_var_fillVar(msh, newCond);
        if (word_compare(altCond, newCond) == 0) {
            break;
        };
    };
    // printf("Cond: %s\n", newCond);
    // printf("newText: %s\n", newText);
    replaceS(newText, ";", "&/break//");
    replace(newText, "_", " ");
    replaceS(newText, "&/underscore//", "_");

    if (find(repl, "def()") != 0) {
        replaceS(newCond, "def()", "");
        msh_command_spezial_def(msh, newCond, newText);
        return 1;
    } else if (find(repl, "val()") != 0) {
        replaceS(newCond, "val()", "");
        msh_command_spezial_val(msh, newCond, newText);
        return 1;
    } else if (find(repl, "if()") != 0) {
        replaceS(newCond, "if()", "");
        msh_command_spezial_if(msh, newCond, newText);
        return 1;
    } else if (find(repl, "for()") != 0) {
        replaceS(newCond, "for()", "");
        msh_command_spezial_for(msh, newCond, newText);
        return 1;
    } else if (find(repl, "foreach()") != 0) {
        replaceS(newCond, "foreach()", "");
        msh_command_spezial_foreach(msh, newCond, newText);
        return 1;
    } else if (find(repl, "forcount()") != 0) {
        replaceS(newCond, "forcount()", "");
        msh_command_spezial_forcount(msh, newCond, newText);
        return 1;
    } else if (find(repl, "while()") != 0) {
        replaceS(newCond, "while()", "");
        msh_command_spezial_while(msh, newCond, newText);
        return 1;
    };
    return 0;
};

void msh_command_spezial_def(msh_info * msh, char Cond[], char text[]) {
    // msh_push_Var(text, Cond);
    msh_var_push(msh, text, Cond);
};
void msh_command_spezial_val(msh_info * msh, char Cond[], char text[]) {
    // get var
    int local = 0;
    if (IN_FUNC && msh_func_get_local_Var_index(text, msh->stack) != -1) {
        superstring textS = msh_func_get_local_Var(text, msh->stack);
        s_stringify(textS, text);
        local = 1;
    } else {
        // msh_get_Var(text, text);
        const char * temp = msh_var_getByName(msh, text);
        word_copy(text, temp);
    }

    // replace
    replaceS(text, "=", "&/equals//");
    replaceS(text, "\r", "\\r");
    replaceS(text, "\n", "\\n");

    // update var
    if (local) {
        msh_func_update_local_Var(Cond, text, msh->stack);
    } else {
        // char var[VAR_MAXCHAR];
        // int index = msh_get_Var(Cond, var);
        int index = msh_var_getIndexByName(msh, Cond);
        if (index == -1) {
            // msh_push_Var(text, Cond);
            msh_var_push(msh, text, Cond);
        } else {
            // word_copy(VAR_SPEICHER[index], text);
            msh_var_updateByIndex(msh, text, index);
        };
    }
}
int msh_check_if(char Cond[]) {
    // Bedingung 1 : "=="
    char ** Bed1;
    int Bed_Anzahl = split(Cond, "==", &Bed1);
    if (Bed_Anzahl != 0) {
        // puts(Bed1[0]);
        // puts(Bed1[1]);
        if (word_compare(Bed1[0], Bed1[1]) == 0) {
            freeWordArr(Bed1, Bed_Anzahl);
            return 1;
        };
        freeWordArr(Bed1, Bed_Anzahl);
        return 0;
    };
    freeWordArr(Bed1, Bed_Anzahl);
    // Bedingung 2 : "!=""
    char ** Bed2;
    Bed_Anzahl = split(Cond, "!=", &Bed2);
    if (Bed_Anzahl != 0) {
        if (word_compare(Bed2[0], Bed2[1]) != 0) {
            freeWordArr(Bed2, Bed_Anzahl);
            return 1;
        };
        freeWordArr(Bed2, Bed_Anzahl);
        return 0;
    };
    freeWordArr(Bed2, Bed_Anzahl);
    // Bedingung 3 : "<=""
    char ** Bed3;
    Bed_Anzahl = split(Cond, "<=", &Bed3);
    if (Bed_Anzahl != 0) {
        if (atof(Bed3[0]) <= atof(Bed3[1])) {
            freeWordArr(Bed3, Bed_Anzahl);
            return 1;
        };
        freeWordArr(Bed3, Bed_Anzahl);
        return 0;
    };
    freeWordArr(Bed3, Bed_Anzahl);
    // Bedingung 4 : ">=""
    char ** Bed4;
    Bed_Anzahl = split(Cond, ">=", &Bed4);
    if (Bed_Anzahl != 0) {
        if (atof(Bed4[0]) >= atof(Bed4[1])) {
            freeWordArr(Bed4, Bed_Anzahl);
            return 1;
        };
        freeWordArr(Bed4, Bed_Anzahl);
        return 0;
    };
    freeWordArr(Bed4, Bed_Anzahl);
    // Bedingung 5 : "<""
    char ** Bed5;
    Bed_Anzahl = split(Cond, "<", &Bed5);
    if (Bed_Anzahl != 0) {
        if (atof(Bed5[0]) < atof(Bed5[1])) {
            freeWordArr(Bed5, Bed_Anzahl);
            return 1;
        };
        freeWordArr(Bed5, Bed_Anzahl);
        return 0;
    };
    freeWordArr(Bed5, Bed_Anzahl);
    // Bedingung 6 : ">""
    char ** Bed6;
    Bed_Anzahl = split(Cond, ">", &Bed6);
    if (Bed_Anzahl != 0) {
        if (atof(Bed6[0]) > atof(Bed6[1])) {
            freeWordArr(Bed6, Bed_Anzahl);
            return 1;
        };
        freeWordArr(Bed6, Bed_Anzahl);
        return 0;
    };
    freeWordArr(Bed6, Bed_Anzahl);
    return 0;
}
void msh_command_spezial_if(msh_info * msh, char Cond[], char text[]) {
    replace(text, " ", "_");
    if (msh_check_if(Cond)) { msh_readZeile(msh, text); }
};
void msh_command_spezial_for(msh_info * msh, char Cond[], char text[]) {
    char ** array;
    int CondTeile = split(Cond, "IN", &array);
    if (CondTeile != 1) { return; };
    // value of iteration var
    // char value[VAR_MAXCHAR];
    // int var_index = msh_get_Var(array[0], value);
    int var_index = msh_var_getIndexByName(msh, array[0]);
    if (var_index == -1) {
        // msh_push_Var("0", array[0]);
        msh_var_push(msh, "0", array[0]);
        var_index = msh_var_getIndexByName(msh, array[0]);
    };
    const char * value = msh_var_getByIndex(msh, var_index);
    // var_index = msh_get_Var(array[0], value);
    replaceS(text, ";", "\n");
    // array length
    char ** arr_Teile;
    int arrTeile = split(array[1], "&/arr//", &arr_Teile);
    freeWordArr(array, CondTeile);
    // word_copy(value, "0");
    msh_var_updateByIndex(msh, "0", var_index);
    // sprintf(value, "%d", 0);

    for (int i = 0; i <= arrTeile;) {
        // printf("%d\n", i);
        // printf("%s\n", arr_Teile[i]);
        if (msh->event.stop == 1) {
            break;
        };
        if (word_compare(arr_Teile[i], "&/null//") == 0 || word_compare(arr_Teile[i], "") == 0) {
            i++;
            // sprintf(value, "%d", i);
            char tempI[intLen(i)+1];
            // word_copy(value, tempI);
            msh_var_updateByIndex(msh, tempI, var_index);
            continue;
        }
        word_copy(VAR_SPEICHER[var_index], value);
        msh_readZeile(msh, text);
        i++;
        // sprintf(value, "%d", i);
        char tempI[intLen(i)+1];
        // word_copy(value, tempI);
        msh_var_updateByIndex(msh, tempI, var_index);
    }
    freeWordArr(arr_Teile, arrTeile);
};
void msh_command_spezial_foreach(msh_info * msh, char Cond[], char text[]) {
    char ** array;
    int CondTeile = split(Cond, "IN", &array);
    if (CondTeile != 1) { return; };
    // value of iteration var
    // char value[VAR_MAXCHAR];
    // int var_index = msh_get_Var(array[0], value);
    int var_index = msh_var_getIndexByName(msh, array[0]);
    if (var_index == -1) {
        // msh_push_Var("0", array[0]);
        msh_var_push(msh, "0", array[0]);
        var_index = msh_var_getIndexByName(msh, array[0]);
    };
    const char * value = msh_var_getByIndex(msh, var_index);
    // var_index = msh_get_Var(array[0], value);
    // array length
    char ** arr_Teile;
    int arrTeile = split(array[1], "&/arr//", &arr_Teile);
    // sprintf(value, "%s", arr_Teile[0]);
    // word_copy(value, arr_Teile[0]);
    msh_var_updateByIndex(msh, arr_Teile[0], var_index);
    freeWordArr(array, CondTeile);

    for (int i = 0; i <= arrTeile;) {
        // printf("%d\n", i);
        // printf("%s\n", arr_Teile[i]);
        if (word_compare(arr_Teile[i], "&/null//") == 0 || word_compare(arr_Teile[i], "") == 0) {
            i++;
            // sprintf(value, "%s",  arr_Teile[i]);
            // word_copy(value, arr_Teile[i]);
            continue;
        }
        word_copy(VAR_SPEICHER[var_index], value);
        msh_readZeile(msh, text);
        i++;
        // sprintf(value, "%s", arr_Teile[i]);
        // word_copy(value, arr_Teile[i]);
        msh_var_updateByIndex(msh, arr_Teile[i], var_index);
    }
    freeWordArr(arr_Teile, arrTeile);
};
void msh_command_spezial_forcount(msh_info * msh, char Cond[], char text[]) {
    char ** array;
    int CondTeile = split(Cond, "IN", &array);
    if (CondTeile != 1 && CondTeile != 0) { return; };
    char newCond[VAR_MAXCHAR];
    // value of iteration var
    // char value[VAR_MAXCHAR];
    int var_index;
    if (CondTeile == 1) {
        // var_index = msh_get_Var(array[0], value);
        var_index = msh_var_getIndexByName(msh, array[0]);
        if (var_index == -1) {
            // msh_push_Var("0", array[0]);
            msh_var_push(msh, "0", array[0]);
            var_index = msh_var_getIndexByName(msh, array[0]);
        };
        // var_index = msh_get_Var(array[0], value);
        word_copy(newCond, array[1]);
    } else {
        word_copy(newCond, Cond);
    };
    freeWordArr(array, CondTeile);

    char ** newCond_Teile;
    int newCondTeile_Anzahl = split(newCond, "TO", &newCond_Teile);
    if (newCondTeile_Anzahl != 1) { return; }
    int Anfang = atoi(newCond_Teile[0]);
    int Ende = atoi(newCond_Teile[1]);
    freeWordArr(newCond_Teile, newCondTeile_Anzahl);

    char value[VAR_MAXCHAR];
    if (CondTeile == 1) {
        intToString(Anfang, value);
        // sprintf(value, "%d", Anfang);
    };

    if (Ende > Anfang) {
        for (int i = Anfang; i <= Ende;) {
            // word_copy(VAR_SPEICHER[var_index], value);
            msh_var_updateByIndex(msh, value, var_index);
            msh_readZeile(msh, text);
            i++;
            if (CondTeile == 1) {
                // sprintf(value, "%d", i);
                intToString(i, value);
            };
        };
    } else if (Anfang > Ende) {
        for (int i = Anfang; i <= Ende;) {
            // word_copy(VAR_SPEICHER[var_index], value);
            msh_var_updateByIndex(msh, value, var_index);
            msh_readZeile(msh, text);
            i++;
            if (CondTeile == 1) {
                // sprintf(value, "%d", i);
                intToString(i, value);
            };
        };
    };
};
void msh_command_spezial_while(msh_info * msh, char Cond[], char text[]) {
    if (msh_check_if(Cond)) { msh_readZeile(msh, text); msh->info.line--; }
}