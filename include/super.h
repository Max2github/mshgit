#pragma once
#include "func.h"
#include "ref.h"

#ifdef __cplusplus
extern "C" {
#endif

// funcs
int  check_Func(msh_info * msh, char ** Script);
void msh_fill_local_Var(char Code[], FUNC_LOCAL_STACK * stack);

// refs / bigdata
bool check_bigdata(msh_info *, char ** Script);

#ifdef __cplusplus
}
#endif