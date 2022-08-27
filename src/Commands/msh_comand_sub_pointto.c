#include "../../include/alg.h"
#include "../../dependencies/words.h"

void msh_command_sub_pointto() {
    char var_name[33];
    int i = 0;
    while (msh_Wert[i] != 'p' || msh_Wert[i+1] != 'o' || msh_Wert[i+2] != 'i' || msh_Wert[i+3] != 'n' || msh_Wert[i+4] != 't' || msh_Wert[i+5] != 'T' || msh_Wert[i+6] != 'o' || msh_Wert[i+7] != '(' || msh_Wert[i+8] != ')') {
        i++;
    };
    i += 9;
    // printf("i: %d\n", i);
    int n = 0;
    while (msh_Wert[i] != '\\' || msh_Wert[i+1] != 'p') {
        if (n > 32) {
            msh_error("Could not find finishing \"\\p\"!\n");
            replaceS(msh_Wert, "pointTo()", "");
            break;
        }
        var_name[n] = msh_Wert[i];
        n++;
        i++;
    };
    var_name[n] = '\0';
    // printf("var_name: %s\n", var_name);
    char repl[35];
    char search[44];
    // sprintf(search, "pointTo()%s\\p", var_name);
    word_copy(search, "pointTo()");
    word_add(search, var_name);
    word_add(search, "\\p");
    word_copy(repl, var_name);
    repl[n] = '(';
    repl[n+1] = ')';
    repl[n+2] = '\0';
    replaceS(msh_Wert, search, repl);
    // printf("Wert: %s\n", msh_Wert);
};