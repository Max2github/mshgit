#pragma once
#include "../dependencies/list.h"
#include "../include/func.h"

#ifdef __cplusplus
extern "C" {
#endif

// msh-functions
int msh_push_Var(char value[], char name[]); // 62
int msh_get_Var(char name[], char saveto[]); // 86
int msh_get_Var_element(int Var_index, char el_name[], char saveto[]); // 113
int msh_fillVar(char Code[]); // 126
int msh_fillObj(char Code[]); // 144
int msh_Breaks(char Code[], FUNC_LOCAL_STACK * stack); // 197

// Commands

// spezial
int msh_command_isSpezial(char Code[], FUNC_LOCAL_STACK * stack); // 355
void msh_command_spezial_def(char Cond[], char text[]); // 373
void msh_command_spezial_if(char Cond[], char text[], FUNC_LOCAL_STACK * stack); // 376
void msh_command_spezial_for(char Cond[], char text[], FUNC_LOCAL_STACK * stack); // 448
void msh_command_spezial_foreach(char Cond[], char text[], FUNC_LOCAL_STACK * stack); // 476
void msh_command_spezial_forcount(char Cond[], char text[], FUNC_LOCAL_STACK * stack); // 477
void msh_command_spezial_while(char Cond[], char text[], FUNC_LOCAL_STACK * stack); // 478

// execute
int msh_readZeile(char Zeile[], FUNC_LOCAL_STACK * stack); // 483
int msh_readScript(char Script[]); // 662
int msh_readFunc(char Script[], FUNC_LOCAL_STACK * stack); // 676
int msh_readFile(char filename[]); // 688

#ifdef __cplusplus
}
#endif