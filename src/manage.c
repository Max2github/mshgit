#include "../include/alg.h"
#include "../dependencies/std.h"
#include "../include/msh.h"

#include "../dependencies/extern.h"
/*
int msh_push_Var(char value[], char name[]) {
    int zeichen = word_len(value);
    int len = word_len(name);
    if (VAR_MAXWORDS < VAR_WORTZAELER + 1) {

    };
    if (VAR_MAXCHAR < VAR_ZEICHENZAELER + zeichen) {

    };
    if (32 < len) {
        MSH_PRINTF(NULL, "Name of Variable \"%s\" is too long (maximum lenght = 32)", name);
        return 1;
    };
    word_copy(VAR_SPEICHER[VAR_WORTZAELER], value);
    word_copy(VAR_NAMES[VAR_WORTZAELER], name);
    VAR_WORTZAELER += 1;
    VAR_ZEICHENZAELER += zeichen;
    return 0;
};
int msh_get_Var(char name[], char saveto[]) {
    int i = 0;
    int ret_empty = 0;
    if (VAR_WORTZAELER == 0) {
        ret_empty = -1;
        return ret_empty;
    };
    for (i = 0; i < VAR_WORTZAELER; i++) {
        if (word_compare(VAR_NAMES[i], name) == 0) {
            ret_empty = i;
            break;
        };
        if (i == VAR_WORTZAELER-1) {
            ret_empty = -1;
        };
    };
    if (ret_empty == -1) {
        word_copy(saveto, "");
        return ret_empty;
    };
    int var_len = word_len(VAR_SPEICHER[i]);
    for (int j = 0; j < var_len; j++) {
        saveto[j] = VAR_SPEICHER[i][j];
    };
    saveto[var_len] = '\0';
    return ret_empty;
};*/

// stop overflow, while copying
bool msh_var_copy_general(msh_info * msh, char * dest, const char * src, unsigned int max, const char * errMsg) {
    if (dest == NULL || src == NULL) { return false; }
    if (word_len(src) >= max) {
        msh_error(msh, errMsg);
        return false;
    }
    word_copy(dest, src);
    return true;
}
bool msh_var_copy_name(msh_info * msh, char * dest, const char * src) {
    return msh_var_copy_general(msh, dest, src, sizeof(VAR_NAMES[0]), "Value of global var exceeds maximum of " STR(sizeof(VAR_SPEICHER[0])) " bytes / characters!");
}
bool msh_var_copy_value(msh_info * msh, char * dest, const char * src) {
    return msh_var_copy_general(msh, dest, src, VAR_MAXCHAR, "Name of Variable is too long! (maximum " STR(sizeof(VAR_NAMES[0])) " bytes / characters)");
}

// add & set / update
bool msh_var_push(msh_info * msh, const char * value, const char * name) {
    if (VAR_WORTZAELER + 1 > VAR_MAXWORDS) {
        msh_error(msh, "The maximum amount of global vars has already been reached!");
        return false;
    }
    int value_len = word_len(value);
    int name_len = word_len(name);
    /*if (value_len > VAR_MAXCHAR) {
        msh_error(msh, "Value of global var exceeds maximum of " STR(sizeof(VAR_SPEICHER[0])) " bytes / characters!");
        return false;
    }
    if (name_len > sizeof(VAR_NAMES[0])) {
        msh_error(msh, "Name of Variable is too long! (maximum " STR(sizeof(VAR_NAMES[0])) " bytes / characters)");
        return false;
    }*/
    // set global var
    // word_copy(VAR_NAMES[VAR_WORTZAELER], name);
    // word_copy(VAR_SPEICHER[VAR_WORTZAELER], value);
    msh_var_copy_name(msh, VAR_NAMES[VAR_WORTZAELER], name);
    msh_var_copy_value(msh, VAR_SPEICHER[VAR_WORTZAELER], value);
    VAR_WORTZAELER++;
    if (value_len > VAR_ZEICHENZAELER) { VAR_ZEICHENZAELER = value_len; } // may abolish this
    return true;
}
bool msh_var_updateByName(msh_info * msh, const char * value, const char * name) {
    int index = msh_var_getIndexByName(msh, name);
    if (index == -1) {
        return msh_var_push(msh, value, name);
    }
    // word_copy(VAR_SPEICHER[index], value);
    // return true;
    return msh_var_copy_value(msh, VAR_SPEICHER[index], value);
}
bool msh_var_updateByIndex(msh_info * msh, const char * value, int index) {
    if (index + 1 > VAR_WORTZAELER || index < 0) {
        msh_error(msh, "upadating global var: index does not exist!");
        return false;
    }
    // word_copy(VAR_SPEICHER[index], value);
    // return true;
    return msh_var_copy_value(msh, VAR_SPEICHER[index], value);
}
// get
const char * msh_var_getByName(msh_info * msh, const char * name) {
    for (int i = 0; i < VAR_WORTZAELER; i++) {
        if (word_compare(VAR_NAMES[i], name) == 0) {
            return VAR_SPEICHER[i];
        }
    }
    return NULL;
}
int msh_var_getIndexByName(msh_info * msh, const char * name) {
    for (int i = 0; i < VAR_WORTZAELER; i++) {
        if (word_compare(VAR_NAMES[i], name) == 0) {
            return i;
        }
    }
    return -1;
}
const char * msh_var_getByIndex(msh_info * msh, int index) {
    return VAR_SPEICHER[index];
}
int msh_get_Var_element(int Var_index, char el_name[], char saveto[]) {
    if (find(VAR_SPEICHER[Var_index], "&/arr//") != 0) {
        char ** arr_Teile;
        int arrTeile = split(VAR_SPEICHER[Var_index], "&/arr//", &arr_Teile);
        for (int i = 0; i < arrTeile+1; i++) {
            if (i == MSH_STRING_TO_INT(el_name)) {
                word_copy(saveto, arr_Teile[i]);
            };
        };
        freeWordArr(arr_Teile, arrTeile);
    };
    return 0;
};
int msh_fillVar(char Code[]) {
    for (int i = 0; i < VAR_WORTZAELER; i++) {
        int name_len = word_len(VAR_NAMES[i]);
        char spl[name_len+3];
        word_copy(spl, VAR_NAMES[i]);
        spl[name_len] = '(';
        spl[name_len+1] = ')';
        spl[name_len+2] = '\0';
        char newCode[VAR_MAXCHAR];
        newCode[0] = '\0';
        word_copy(newCode, Code);
        replaceS(newCode, spl, VAR_SPEICHER[i]);
        // Fehler bei replaceS : spl = "Name()", geschnitten wird aber "n()Name()"!
        word_copy(Code, newCode);
    };
    return 0;
};
int msh_fillObj(char Code[]) {
    for (int i = 0; i < VAR_WORTZAELER; i++) {
        char spl[VAR_MAXCHAR];
        char newCode[VAR_MAXCHAR];
        int ALT = 0;
        int ALT_index = 0;
        int ALT_name_len = 0;

        char ** obj_Teile;
        int objTeile = split(VAR_SPEICHER[i], "&/obj//", &obj_Teile);
        char ** obj_TeileNEU;
        char ** obj_TeileALT;
        int objTeileALT = 0;

        char ** arr_Teile;
        int arrTeile = split(VAR_SPEICHER[i], "&/arr//", &arr_Teile);

        if (objTeile == 0 && arrTeile == 0) {
            freeWordArr(obj_Teile, objTeile);
            freeWordArr(arr_Teile, arrTeile);
            continue;
        } else if (arrTeile != 0) {
            word_copy(spl, VAR_NAMES[i]);
            goto ARR;
        } else if (objTeile != 0) {
            word_copy(spl, VAR_NAMES[i]);
            goto OBJ;
        };

        OBJ: if(ALT == 2) {
            ALT = 0;
            obj_Teile = obj_TeileALT;
            objTeile = objTeileALT;

            // reset latest search - also search for other elements
            int spl_len = word_len(spl);
            spl[spl_len - ALT_name_len - 1] = '\0';
        };
        int u = 0;
        if (ALT == 0) {
            u = ALT_index;
        }
        for (;u < objTeile+1; u++) {
            char ** key_value;
            int keyvalAnzahl = split(obj_Teile[u], ":", &key_value);
            // printWordArr(key_value, 1);

            word_copy(newCode, Code);
            char newSPL[34];
            word_copy(newSPL, ".");
            word_add(newSPL, key_value[0]);
            word_add(spl, newSPL);

            // überprüfen, ob diese Element aufgerufen wird (auch wenn es selber ein Objekt ist)
            char checkSPL[VAR_MAXCHAR];
            word_copy(checkSPL, spl);
            word_add(checkSPL, "()");
            if (find(newCode, checkSPL) != 0) {
                replaceS(newCode, checkSPL, key_value[1]);
                word_copy(Code, newCode);
            };

            char value[VAR_MAXCHAR];
            word_copy(value, key_value[1]);
            // msh_fillObj(value);
            if (find(value, "&/arr//") != 0) {
                ALT = 1;
                ALT_index = u + 1;
                ALT_name_len = word_len(key_value[0]);

                arrTeile = split(value, "&/arr//", &arr_Teile);
                freeWordArr(key_value, keyvalAnzahl);
                goto ARR;
            };
            msh_fillVar(value);
            if (find(value, "&/obj//") != 0) {
                ALT = 1;
                ALT_index = u + 1;
                ALT_name_len = word_len(key_value[0]);

                objTeileALT = objTeile;
                obj_TeileALT = obj_Teile;

                objTeile = split(value, "&/obj//", &obj_TeileNEU);
                obj_Teile = obj_TeileNEU;
                freeWordArr(key_value, keyvalAnzahl);
                goto OBJ;
            }
            // reset latest search - also search for other elements
            int name_len = word_len(key_value[0]);
            int spl_len = word_len(spl);
            spl[spl_len - name_len - 1] = '\0';

            freeWordArr(key_value, keyvalAnzahl);
        };
        if (ALT == 1) {
            ALT++;
            goto OBJ;
        }
        freeWordArr(obj_Teile, objTeile);
        freeWordArr(arr_Teile, arrTeile);
        continue;

        ARR: for (int a = 0; a < arrTeile+1; a++) {
            word_copy(newCode, Code);
            word_copy(spl, VAR_NAMES[i]);
            word_add(spl, ".");
            char tempA[intLen(a)+1];
            intToString(a, tempA);
            word_add(spl, tempA);
            word_add(spl, "()");

            if (find(newCode, spl) == 0) {
                continue;
            };

            replaceS(newCode, spl, arr_Teile[a]);
            word_copy(Code, newCode);
        };
        freeWordArr(obj_Teile, objTeile);
        freeWordArr(arr_Teile, arrTeile);
    };
    return 0;
};
int msh_Breaks(msh_info * msh, char Code[]) {
    char ** Code_Teile;
    int CodeTeile = split(Code, "&/break//", &Code_Teile);
    if (CodeTeile > 0) {
        for (int i = 0; i <= CodeTeile; i++) {
            msh_readZeile(msh, Code_Teile[i]);
        };
        freeWordArr(Code_Teile, CodeTeile);
        return 1;
    };
    freeWordArr(Code_Teile, CodeTeile);
    return 0;
};