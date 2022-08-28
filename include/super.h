#pragma once
#include "func.h"

#ifdef __cplusplus
extern "C" {
#endif

int check_Func(char ** Script);
void msh_fill_local_Var(char Code[], FUNC_LOCAL_STACK * stack);

#ifdef __cplusplus
}
#endif