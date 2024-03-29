#include "../../include/msh.h"
#include "../../include/alg.h"
#include "../../include/super.h"

#include "../../dependencies/extern.h"

int check_Func(msh_info * msh, char ** Script) {
    char * Zeile = Script[msh->info.line-1];
    if (find(Zeile, "func()")) {
        replaceS(Zeile, "func()", "");

        replaceS(Zeile, " ", "");
        replace(Zeile, "_", " ");
        replaceS(Zeile, "&/underscore//", "_");

        superstring func_name = s_init(Zeile);
        FUNC_NAMES = s_arr_addFirst(FUNC_NAMES, func_name);
        
        superstring code = NULL;
        msh->info.line++;
        code = s_addStr(code, Script[msh->info.line-1]);
        msh->info.line++;
        while (!find(Script[msh->info.line-1], "funcEnd()")) {
            s_addLast(code, '\n', 1);
            code = s_addStr(code, Script[msh->info.line-1]);
            msh->info.line++;
        }
        FUNC_SPEICHER = s_arr_addFirst(FUNC_SPEICHER, code);
        return 1;
    }
    return 0;
}
void msh_fill_local_Var(char Code[], FUNC_LOCAL_STACK * stack) {
    s_arr temp = stack->VAR_NAMES;
    while (temp != NULL) {
        superstring name = temp->element;
        int name_len = s_len(name);
        char spl[name_len+3];
        s_stringify(name, spl);

        char var[VAR_MAXCHAR];
        s_stringify(msh_func_get_local_Var(spl, stack), var);

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

void msh_fill_local_Obj(char Code[], FUNC_LOCAL_STACK * stack) {
    // int stack_id = msh_func_stacks_count(FUNC_STACKS) - 1;

    // s_arr temp = *msh_func_get_func_var_names_pointer(stack_id);
    /*s_arr temp = stack->VAR_NAMES;
    while (temp != NULL) {
        superstring name = temp->element;
        
        int name_len = s_len(name);
        char spl[name_len+3];
        s_stringify(name, spl);

        char var[VAR_MAXCHAR];
        s_stringify(msh_func_get_local_Var(spl, stack), var);

        spl[name_len] = '.';
        spl[name_len+1] = '(';
        spl[name_len+2] = ')';
        spl[name_len+3] = '\0';

        char newCode[VAR_MAXCHAR];
        word_copy(newCode, Code);
        replaceS(newCode, spl, var);
        word_copy(Code, newCode);

        temp = temp->next;
    }; */
}

// refs / bigdata

bool check_bigdata(msh_info * msh, char ** Script) {
    char * Zeile = Script[msh->info.line-1];
    if (find(Zeile, "bigdata()")) {
        replaceS(Zeile, "bigdata()", "");

        replaceS(Zeile, " ", "");
        replace(Zeile, "_", " ");
        replaceS(Zeile, "&/underscore//", "_");

        typedef sString check_bigdata_string;
        check_bigdata_string * code = MSH_MALLOC(sizeof(check_bigdata_string));
        *code = sString_create();
        msh->info.line++;

        index64 nbytes = 0;

        #define CONTINUE \
            msh->info.line++; \
            toAdd = Script[msh->info.line-1]; \
            continue;

        /*const */char * toAdd = Script[msh->info.line-1];
        while (toAdd != NULL && !find(toAdd, "bigdataEnd()")) {
            replaceS(toAdd, "\\r", "\r");
            replaceS(toAdd, "\\n", "\n");

            index64 bytesNow = word_len(toAdd);
            if (bytesNow == 0) { CONTINUE; }

            sString_addStr(code, (const sString_char_t *) toAdd, bytesNow);

            nbytes += bytesNow;

            CONTINUE;
        }

        union msh_ref_data data;
        data.pointer.sstr = code;
        index32 id = msh_ref_add(msh, data, msh_ref_type_SMARTSTRING, nbytes);

        char idStr[msh_ref_id_as_string_len(id)];
        msh_ref_id_to_string(id, idStr);

        if (msh->info.in_func) {
            msh_func_update_local_Var(Zeile, idStr, msh->stack);
        } else {
            msh_var_updateByName(msh, idStr, Zeile);
        }

        return true;
    }
    return false;
}