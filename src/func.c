#include "../include/func.h"
#include "../include/alg.h"
#include "../dependencies/std.h"
#include "../dependencies/words.h"
#include "../include/msh.h"

int msh_func_stacks_count(FUNC_STACK head) {
    int i = 0;
    for (; head != NULL; i++) { head = head->next; }
    return i;
}
void msh_func_stack_print(FUNC_STACK head, int stack_id) {
    puts("READ");
        printf("Stack %d:\n", stack_id);
        s_arr_print(*msh_func_get_func_var_names_pointer(stack_id));
        s_arr_print(*msh_func_get_func_var_speicher_pointer(stack_id));
    puts("READ END");
}
void msh_func_stacks_print(FUNC_STACK head) {
    puts("READ");
    int i = 0;
    for (;head != NULL; i++) {
        printf("Stack %d:\n", i);
        s_arr_print(head->VAR_NAMES);
        s_arr_print(head->VAR_SPEICHER);
        head = head->next;
    }
    puts("READ END");
}
FUNC_STACK msh_func_create_new_stack(FUNC_STACK head) {
    FUNC_STACK new_FUNC_stack = (FUNC_STACK) malloc(sizeof(FUNC_ONE_STACK));
    new_FUNC_stack->VAR_NAMES = NULL;
    new_FUNC_stack->VAR_SPEICHER = NULL;
    if (head == NULL) {
        new_FUNC_stack->next = NULL;
        return new_FUNC_stack;
    }
    new_FUNC_stack->next = NULL;
    FUNC_STACK temp = head;
    for (;temp->next != NULL; temp = temp->next);
    temp->next = new_FUNC_stack;
    return head;
}
FUNC_STACK msh_func_remove_last(FUNC_STACK head) {
    if (head == NULL) { return NULL; }
    if (head->next == NULL) {
        s_arr_free(head->VAR_NAMES);
        s_arr_free(head->VAR_SPEICHER);
        free(head);
        return NULL;
    }
    FUNC_STACK temp = head;
    for (;temp->next->next != NULL; temp = temp->next);
    s_arr_free(temp->next->VAR_NAMES);
    s_arr_free(temp->next->VAR_SPEICHER);
    free(temp->next);
    temp->next = NULL;
    return head;
}
void msh_func_remove_all() {
    while (FUNC_STACKS != NULL){
        FUNC_STACKS = msh_func_remove_last(FUNC_STACKS);
    }
}
s_arr * msh_func_get_func_var_names_pointer(int func_speicher_index) {
    FUNC_STACK temp = FUNC_STACKS;
    for (int i = 0; i < func_speicher_index; i++) { temp = temp->next; }
    return &(temp->VAR_NAMES);
}
s_arr * msh_func_get_func_var_speicher_pointer(int func_speicher_index) {
    FUNC_STACK temp = FUNC_STACKS;
    for (int i = 0; i < func_speicher_index; i++) { temp = temp->next; }
    return &(temp->VAR_SPEICHER);
}
int msh_func_get_local_Var_index(const char name[], int func_speicher_index) {
    superstring nameS = s_init(name);
    s_arr func_var_names = *msh_func_get_func_var_names_pointer(func_speicher_index);
    int index = s_arr_findFirstIndex(func_var_names, nameS);
    s_free(nameS);
    return index;
}
superstring msh_func_get_local_Var(const char name[], int func_speicher_index) {
    int index = msh_func_get_local_Var_index(name, func_speicher_index);
    if (index == -1) {
        return NULL;
    }
    s_arr func_var_speicher = *msh_func_get_func_var_speicher_pointer(func_speicher_index);
    superstring valueS = s_arr_getEl(func_var_speicher, index);
    return valueS;
}
void msh_func_set_local_Var(const char name[], const char value[], int func_speicher_index) {
    s_arr * func_var_names_pointer = msh_func_get_func_var_names_pointer(func_speicher_index);
    s_arr * func_var_speicher_pointer = msh_func_get_func_var_speicher_pointer(func_speicher_index);
    *func_var_names_pointer = s_arr_addFirst(*func_var_names_pointer, s_init(name));
    *func_var_speicher_pointer = s_arr_addFirst(*func_var_speicher_pointer, s_init(value));
}
void msh_func_update_local_Var(const char name[], const char value[], int func_speicher_index) {
    superstring nameS = s_init(name);
    int index = msh_func_get_local_Var_index(name, func_speicher_index);
    s_free(nameS);
    if (index == -1) {
        msh_func_set_local_Var(name, value, func_speicher_index);
        return;
    }
    s_arr temp = *msh_func_get_func_var_speicher_pointer(func_speicher_index);
    for (int i = 0; i < index; i++) { temp = temp->next; }
    s_free(temp->element);
    temp->element = s_init(value);
}
superstring msh_func_get_fullname(const char * name) {
    int i = 0;
    s_arr temp = FUNC_NAMES;
    new_superstring search = s_init(name);
    while (temp != NULL) {
        const new_superstring spl = s_init("(");
        new_s_arr nameANDargsSEARCH = s_split(search, spl);
        new_s_arr nameANDargsTEMP = s_split(temp->element, spl);
        s_free(spl);
        // s_free(search);
        if (s_compare(nameANDargsTEMP->element, nameANDargsSEARCH->element) == 0) {
            s_arr_free(nameANDargsSEARCH);
            s_arr_free(nameANDargsTEMP);
            s_free(search);
            return temp->element;
        }
        s_arr_free(nameANDargsSEARCH);
        s_arr_free(nameANDargsTEMP);
        temp = temp->next;
        i++;
    }
    s_free(search);
    return NULL;
}

int msh_func_get_index(const char name[]) {
    superstring fullname = msh_func_get_fullname(name);
    int index = s_arr_findFirstIndex(FUNC_NAMES, fullname);
    return index;
}
superstring msh_func_get_code(const char name[]) {
    if (find(name, "(")) {
        char ** wordArr;
        int Teile = split(name, "(", &wordArr);
        wordArr[1][word_len(wordArr[1]) - 1] = '\0';

        superstring result = s_arr_getEl(FUNC_SPEICHER, msh_func_get_index(wordArr[0]));
        freeWordArr(wordArr, Teile);
        return result;
    }
    return s_arr_getEl(FUNC_SPEICHER, msh_func_get_index(name));
}

void msh_func_call(const char * name) {
    FUNC_STACKS = msh_func_create_new_stack(FUNC_STACKS);
    int stack_id = msh_func_stacks_count(FUNC_STACKS) - 1;
    // printf("stack_id: %d\n", stack_id);
    s_arr * func_var_names_pointer = msh_func_get_func_var_names_pointer(stack_id);
    s_arr * func_var_speicher_pointer = msh_func_get_func_var_speicher_pointer(stack_id);

    // getting full name of the function and required arguments
    superstring fullname = msh_func_get_fullname(name); // full name
    new_superstring spl = s_init("(");
    new_s_arr nameANDargs = s_split(fullname, spl); // 0: name, 1: args + ')'
    s_free(spl);
    if (s_arr_len(nameANDargs) > 1) {
        superstring name = s_arr_getEl(nameANDargs, 0); // just for a better understanding
        superstring args = s_arr_getEl(nameANDargs, 1);
        s_removeChar(args, s_len(args)-1); // remove the ')' at the end

        // defining required arguments ("nameless arguments")
        spl = s_init(",");
        new_s_arr argList = s_split(args, spl); // array of arguments
        s_free(spl);
        for (s_arr temp = argList; temp != NULL; temp = temp->next) {
            superstring nameS = temp->element; // the current argument / just for better understanding

            // define the names (without the values) in the stack
            *func_var_names_pointer = s_arr_addFirst(*func_var_names_pointer, s_copy(nameS));
        }
        s_arr_free(argList);
    }
    s_arr_free(nameANDargs);

    // getting and preparing code
    superstring codeS = msh_func_get_code(name);
    char codeStr[s_len(codeS) + 1];
    s_stringify(codeS, codeStr);

    // initializing arguments
    if (find(name, "(")) {
        char ** wordArr;
        int Teile = split(name, "(", &wordArr); // wordArr[0] = name, wordArr[1] = args + ')'
        wordArr[1][word_len(wordArr[1]) - 1] = '\0'; // remove the ')' at the end

        char ** argArray;
        int argAnzahl = split(wordArr[1], ",", &argArray); // make an array of arguments
        for (int i = 0; i <= argAnzahl; i++) {
            if (find(argArray[i], ":")) {
                char ** argTeile;
                int argTeile_Anzahl = split(argArray[i], ":", &argTeile);
                if (s_arr_len(*func_var_speicher_pointer) != s_arr_len(*func_var_names_pointer)) {
                    *func_var_speicher_pointer = s_arr_addFirst(*func_var_speicher_pointer, s_init(argTeile[1]));
                } else {
                    msh_func_update_local_Var(argTeile[0], argTeile[1], stack_id);
                }
                freeWordArr(argTeile, argTeile_Anzahl);
            } else if (s_arr_len(*func_var_speicher_pointer) < s_arr_len(*func_var_names_pointer)) {
                *func_var_speicher_pointer = s_arr_addFirst(*func_var_speicher_pointer, s_init(argArray[i]));
            } else {
                msh_error("too many nameless arguments");
            }
        }
        freeWordArr(argArray, argAnzahl);

        freeWordArr(wordArr, Teile);
    }
    if (s_arr_len(*func_var_speicher_pointer) != s_arr_len(*func_var_names_pointer)) {
        // s_arr_print(FUNC_VAR_NAMES);
        // s_arr_print(FUNC_VAR_SPEICHER);
        msh_error("count of required arguments and provided arguments is not even!\n!! Abording");
        msh_func_remove_last(FUNC_STACKS);
        return;
    }
    // executing function
    msh_readFunc(codeStr);
}