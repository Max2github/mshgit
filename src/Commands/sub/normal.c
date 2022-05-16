#include "../../../include/msh.h"
#include "../../../include/alg.h"
#include "../../../dependencies/words.h"
#include "../../../dependencies/std.h"
#include "../../../dependencies/mathe.h"

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
void msh_command_sub_array() {
    // printf("Wert: %s\n", msh_Wert);
    char ** array;
    int arrTeile = split(msh_Wert, "IN", &array);
    if (arrTeile > 0) {
        if(word_compare(array[1], "&/null//") == 0 || word_compare(array[1], "") == 0) {
            word_copy(msh_Wert, "&/null//&/arr//&/null//");
            return;
        };

        word_copy(msh_Wert, array[1]);
        replaceS(msh_Wert, array[0], "&/arr//");

        if (find(msh_Wert, "&/arr//") == 0) {
            word_add(msh_Wert, "&/arr//&/null//");
        } else if (word_compare(msh_Wert, "&/arr//") == 0) {
            // sprintf(msh_Wert, "&/null//&/arr//&/null//");
            word_copy(msh_Wert, "&/null//&/arr//&/null//");
        } else {
            replaceS(msh_Wert, "&/arr//&/arr//", "&/arr//&/null//&/arr//");
        };
        // printf("arr 0 : \"%s\"\n", array[0]);
        // printf("arr 1 : \"%s\"\n", array[1]);
        // printf("Wert: %s\n", msh_Wert);
        // printf("\n");
    };
    freeWordArr(array, arrTeile);
};
void msh_command_sub_object() {
    char ** object;
    int objTeile = split(msh_Wert, "IN", &object);
    if (objTeile > 0) {
        word_copy(msh_Wert, object[1]);
        replaceS(msh_Wert, object[0], "&/obj//");
    }
    freeWordArr(object, objTeile);
};
void msh_command_sub_random() {
    char ** zahlen;
    int zahlTeile = split(msh_Wert, "&/arr//", &zahlen);
    int MIN = atoi(zahlen[0]);
    int MAX = atoi(zahlen[1]);
    int Komma = atoi(zahlen[2]);
    double r = Random(MIN, MAX, Komma);
    char sr[VAR_MAXCHAR];
    // char selector[] = "%.Kommalf";
    // replaceS(selector, "Komma", zahlen[2]);
    // sprintf(sr, selector, r);
    doubleToString(r, atoi(zahlen[2]), sr);

    word_copy(msh_Wert, sr);
    // printf("%s\n", msh_Wert);
    freeWordArr(zahlen, zahlTeile);
};
void msh_command_sub_randomin() {
    char ** elems;
    int elemTeile = split(msh_Wert, "&/arr//", &elems);
    char arr[] = "0&/arr//%d&/arr//0";
    // sprintf(arr, "0&/arr//%d&/arr//0", elemTeile);
    word_copy(arr, "0&/arr//");
    intToString(elemTeile, arr+8); // not very beautifull
    word_copy(msh_Wert, arr);
    msh_command_sub_random();
    word_copy(msh_Wert, elems[atoi(msh_Wert)]);
    freeWordArr(elems, elemTeile);
};
void msh_command_sub_input() {
    // printf("%s", msh_Wert);
    msh_command_main_print(); // will print msh_Wert
    // scanf(" %s", msh_Wert);
    fgets(msh_Wert, VAR_MAXCHAR-1, stdin); // this could cause problems
    replaceS(msh_Wert, "\n", "");
    replaceS(msh_Wert, "=", "&/equals//");
};
void msh_command_sub_math() {
    double erg = rechnen(msh_Wert);
    // Kommastellen mit 0 am Ende entfernen
    char newWert[VAR_MAXCHAR];
    char repl[VAR_MAXCHAR];
    // sprintf(newWert, "%lf", erg);
    doubleToString(erg, 6, newWert);
    // sprintf(repl, "%d", (int) erg);
    intToString((int) erg, repl);
    replaceS(newWert, repl, "");
    int last = word_len(newWert)-1;
    while (newWert[last] == '0') {
        newWert[last] = '\0';
        last--;
    };
    if (last == 0) {
        newWert[0] = '\0';
    };
    // sprintf(msh_Wert, "%s%s", repl, newWert);
    word_copy(msh_Wert, repl);
    if (word_len(newWert) > 0) {
        word_add(msh_Wert, newWert);
    }
};
void msh_command_sub_row() {
    // sprintf(msh_Wert, "%d", msh_Script_it);
    intToString(msh_Script_it, msh_Wert);
};
void msh_command_sub_ascii() {
    // sprintf(msh_Wert, "%d", (int) msh_Wert[0]);
    intToString((int) msh_Wert[0], msh_Wert);
};