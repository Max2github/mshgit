#include "../../include/msh.h"
#include "../../include/alg.h"
#include "../../dependencies/words.h"

int check_Func(char ** Script);
void msh_fill_local_Var(char Code[]);

int check_Func(char ** Script) {
    char * Zeile = Script[msh_Script_it];
    if (find(Zeile, "func()")) {
        replaceS(Zeile, "func()", "");

        replaceS(Zeile, " ", "");
        replace(Zeile, "_", " ");
        replaceS(Zeile, "&/underscore//", "_");

        superstring func_name = s_init(Zeile);
        FUNC_NAMES = s_arr_addFirst(FUNC_NAMES, func_name);
        
        superstring code = NULL;
        msh_Script_it++;
        code = s_addStr(code, Script[msh_Script_it]);
        msh_Script_it++;
        while (!find(Script[msh_Script_it], "funcEnd()")) {
            s_addLast(code, '\n', 1);
            code = s_addStr(code, Script[msh_Script_it]);
            msh_Script_it++;
        }
        FUNC_SPEICHER = s_arr_addFirst(FUNC_SPEICHER, code);
        return 1;
    }
    return 0;
}
void msh_fill_local_Var(char Code[]) {
    int stack_id = msh_func_stacks_count(FUNC_STACKS) - 1;

    s_arr temp = *msh_func_get_func_var_names_pointer(stack_id);
    while (temp != NULL) {
        superstring name = temp->element;
        int name_len = s_len(name);
        char spl[name_len+3];
        s_stringify(name, spl);

        char var[VAR_MAXCHAR];
        s_stringify(msh_func_get_local_Var(spl, stack_id), var);

        spl[name_len] = '(';
        spl[name_len+1] = ')';
        spl[name_len+2] = '\0';

        char newCode[VAR_MAXCHAR];
        word_copy(newCode, Code);
        replaceS(newCode, spl, var);
        word_copy(Code, newCode);

        temp = temp->next;
    };
}

void msh_fill_local_Obj(char Code[]) {
    int stack_id = msh_func_stacks_count(FUNC_STACKS) - 1;

    s_arr temp = *msh_func_get_func_var_names_pointer(stack_id);
    while (temp != NULL) {
        superstring name = temp->element;
        
        int name_len = s_len(name);
        char spl[name_len+3];
        s_stringify(name, spl);

        char var[VAR_MAXCHAR];
        s_stringify(msh_func_get_local_Var(spl, stack_id), var);

        spl[name_len] = '(';
        spl[name_len+1] = ')';
        spl[name_len+2] = '\0';

        char newCode[VAR_MAXCHAR];
        word_copy(newCode, Code);
        replaceS(newCode, spl, var);
        word_copy(Code, newCode);

        temp = temp->next;
    };
}