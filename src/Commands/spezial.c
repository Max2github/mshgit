#include "../../include/msh.h"
#include "../../include/super.h"
#include "../../include/alg.h"
#include "../../dependencies/words.h"
#include "../../dependencies/std.h"

int msh_command_isSpezial(char Code[]) {
    if (find(Code, ":") == 0) {
        return 0;
    };
    char ** Code_Teile;
    int CodeTeile = split(Code, ":", &Code_Teile);
    char newText[VAR_MAXCHAR];
    char repl[VAR_MAXCHAR];
    join((const char **) Code_Teile, CodeTeile, ":", newText);
    sprintf(repl, "%s:", Code_Teile[0]);
    replaceS(newText, repl, "");
    char newCond[VAR_MAXCHAR];
    word_copy(newCond, Code_Teile[0]);
    freeWordArr(Code_Teile, CodeTeile);

    while (IN_FUNC) {
        char altCond[VAR_MAXCHAR];
        word_copy(altCond, newCond);
        msh_fill_local_Var(newCond);
        if (word_compare(altCond, newCond) == 0) {
            break;
        };
    };
    while (1) {
        char altCond[VAR_MAXCHAR];
        word_copy(altCond, newCond);
        msh_fillObj(newCond);
        msh_fillVar(newCond);
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
        msh_command_spezial_def(newCond, newText);
        return 1;
    } else if (find(repl, "if()") != 0) {
        replaceS(newCond, "if()", "");
        msh_command_spezial_if(newCond, newText);
        return 1;
    } else if (find(repl, "for()") != 0) {
        replaceS(newCond, "for()", "");
        msh_command_spezial_for(newCond, newText);
        return 1;
    } else if (find(repl, "foreach()") != 0) {
        replaceS(newCond, "foreach()", "");
        msh_command_spezial_foreach(newCond, newText);
        return 1;
    } else if (find(repl, "forcount()") != 0) {
        replaceS(newCond, "forcount()", "");
        msh_command_spezial_forcount(newCond, newText);
        return 1;
    } else if (find(repl, "while()") != 0) {
        replaceS(newCond, "while()", "");
        msh_command_spezial_while(newCond, newText);
        return 1;
    };
    return 0;
};

void msh_command_spezial_def(char Cond[], char text[]) {
    msh_push_Var(text, Cond);
};
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
void msh_command_spezial_if(char Cond[], char text[]) {
    replace(text, " ", "_");
    if (msh_check_if(Cond)) { msh_readZeile(text); }
};
void msh_command_spezial_for(char Cond[], char text[]) {
    char ** array;
    int CondTeile = split(Cond, "IN", &array);
    if (CondTeile != 1) { return; };
    // value of iteration var
    char value[VAR_MAXCHAR];
    int var_index = msh_get_Var(array[0], value);
    if (var_index == -1) {
        msh_push_Var("0", array[0]);
    };
    var_index = msh_get_Var(array[0], value);
    replaceS(text, ";", "\n");
    // array length
    char ** arr_Teile;
    int arrTeile = split(array[1], "&/arr//", &arr_Teile);
    freeWordArr(array, CondTeile);
    sprintf(value, "%d", 0);

    for (int i = 0; i <= arrTeile; i = i) {
        // printf("%d\n", i);
        // printf("%s\n", arr_Teile[i]);
        if (msh_STOP == 1) {
            break;
        };
        if (word_compare(arr_Teile[i], "&/null//") == 0 || word_compare(arr_Teile[i], "") == 0) {
            i++;
            sprintf(value, "%d", i);
            continue;
        }
        word_copy(VAR_SPEICHER[var_index], value);
        msh_readZeile(text);
        i++;
        sprintf(value, "%d", i);
    }
    freeWordArr(arr_Teile, arrTeile);
};
void msh_command_spezial_foreach(char Cond[], char text[]) {
    char ** array;
    int CondTeile = split(Cond, "IN", &array);
    if (CondTeile != 1) { return; };
    // value of iteration var
    char value[VAR_MAXCHAR];
    int var_index = msh_get_Var(array[0], value);
    if (var_index == -1) {
        msh_push_Var("0", array[0]);
    };
    var_index = msh_get_Var(array[0], value);
    // array length
    char ** arr_Teile;
    int arrTeile = split(array[1], "&/arr//", &arr_Teile);
    sprintf(value, "%s", arr_Teile[0]);
    freeWordArr(array, CondTeile);

    for (int i = 0; i <= arrTeile; i = i) {
        // printf("%d\n", i);
        // printf("%s\n", arr_Teile[i]);
        if (word_compare(arr_Teile[i], "&/null//") == 0 || word_compare(arr_Teile[i], "") == 0) {
            i++;
            sprintf(value, "%s",  arr_Teile[i]);
            continue;
        }
        word_copy(VAR_SPEICHER[var_index], value);
        msh_readZeile(text);
        i++;
        sprintf(value, "%s", arr_Teile[i]);
    }
    freeWordArr(arr_Teile, arrTeile);
};
void msh_command_spezial_forcount(char Cond[], char text[]) {
    char ** array;
    int CondTeile = split(Cond, "IN", &array);
    if (CondTeile != 1 && CondTeile != 0) { return; };
    char newCond[VAR_MAXCHAR];
    // value of iteration var
    char value[VAR_MAXCHAR];
    int var_index;
    if (CondTeile == 1) {
        var_index = msh_get_Var(array[0], value);
        if (var_index == -1) {
            msh_push_Var("0", array[0]);
        };
        var_index = msh_get_Var(array[0], value);
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
    if (CondTeile == 1) {
        sprintf(value, "%d", Anfang);
    };

    if (Ende > Anfang) {
        for (int i = Anfang; i <= Ende; i = i) {
            word_copy(VAR_SPEICHER[var_index], value);
            msh_readZeile(text);
            i++;
            if (CondTeile == 1) {
                sprintf(value, "%d", i);
            };
        };
    } else if (Anfang > Ende) {
        for (int i = Anfang; i <= Ende; i = i) {
            word_copy(VAR_SPEICHER[var_index], value);
            msh_readZeile(text);
            i++;
            if (CondTeile == 1) {
                sprintf(value, "%d", i);
            };
        };
    };
};
void msh_command_spezial_while(char Cond[], char text[]) {
    if (msh_check_if(Cond)) { msh_readZeile(text); msh_Script_it--; }
}