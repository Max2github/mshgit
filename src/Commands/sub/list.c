#include "../../../include/msh.h"
#include "../../../include/alg.h"
#include "../../../dependencies/words.h"
#include "../../../dependencies/std.h"

void msh_command_sub_list_toString() {
    char ** wordArr;
    int Teile = split((const char *) msh_Wert, "IN", &wordArr);
    if (Teile == 0) { freeWordArr(wordArr, Teile); }
    else { word_copy(msh_Wert, wordArr[Teile]); }

    list List = msh_getListByName(msh_Wert);
    // list_select_toStr(msh_Wert, select_All, List, &msh_list_toStr_condAll, NULL);
    if (Teile == 0) { list_toStr(List, msh_Wert, NULL); }
    else {
        char ** sepArr;
        int sepAnzahl = split(wordArr[0], "&/arr//", &sepArr);
        list sepList = list_addFirst(NULL, String, sepArr[0], End);
        for (int sepCount = 1; sepCount <= sepAnzahl; sepCount++) {
            list_addLast(sepList, String, sepArr[sepCount], End);
        }
        freeWordArr(sepArr, sepAnzahl);
        list_toStr(List, msh_Wert, sepList);
        list_free(sepList);
        freeWordArr(wordArr, Teile);
    }
}
void msh_command_sub_list_getFirst() {
    char ** wordArr;
    int Teile = split(msh_Wert, ":", &wordArr);
    if (Teile != 2) {
        freeWordArr(wordArr, Teile);
        return;
    }
    char * name = wordArr[0];
    char * type = wordArr[1];
    char * value = wordArr[2];
    list toMod = msh_getListByName(name);
    if (toMod == NULL) {
        // printf("Error: List \"%s\" not found!\n", name);
        word_copy(msh_Wert, "NULL");
        freeWordArr(wordArr, Teile);
        return;
    }

    if (word_compare(type, "Index") == 0) {
        int index = atoi(value);
        if (index >= list_node_len(toMod)) {
            word_copy(msh_Wert, "NULL");
            freeWordArr(wordArr, Teile);
            return;
        }
        list_element_pointer el = list_seek(toMod, index);
        list_element_sprint(el, msh_Wert, NULL);
    } else if (word_compare(type, "Type") == 0) {
        
    } else if (word_compare(type, "Type&Value") == 0) {
        
    } else {
        printf("Error: Option \"%s\" not found! Please only enter \"Index\", \"Type\" or \"Type&Value\".\n", type);
    }
    freeWordArr(wordArr, Teile);
}
void msh_command_sub_list_len() {
    sprintf(msh_Wert, "%d", list_node_len(msh_getListByName(msh_Wert)));
}