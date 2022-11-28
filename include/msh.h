#ifndef MSH_H
#define MSH_H

#include "../include/alg.h"
#include "../include/func.h"

#ifdef __cplusplus
extern "C" {
#endif

// msh-functions
// deprecated
//int msh_push_Var(char value[], char name[]); // 62
//int msh_get_Var(char name[], char saveto[]); // 86

bool msh_var_copy(msh_info *, char * dest, const char * src);

bool msh_var_push(msh_info *, const char * value, const char * name);
bool msh_var_updateByName(msh_info *, const char * value, const char * name);
bool msh_var_updateByIndex(msh_info *, const char * value, int index);
const char * msh_var_getByName(msh_info *, const char * name);
int msh_var_getIndexByName(msh_info *, const char * name);
const char * msh_var_getByIndex(msh_info *, int index);
int msh_get_Var_element(int Var_index, char el_name[], char saveto[]); // 113

int msh_fillVar(char Code[]); // 126
int msh_fillObj(char Code[]); // 144

int msh_Breaks(msh_info * msh, char Code[]); // 197

// Commands

// spezial
int msh_command_isSpezial(msh_info *, char Code[]); // 355
void msh_command_spezial_val(msh_info *, char Cond[], char text[]);
void msh_command_spezial_def(msh_info *, char Cond[], char text[]); // 373
void msh_command_spezial_if(msh_info *, char Cond[], char text[]); // 376
void msh_command_spezial_for(msh_info *, char Cond[], char text[]); // 448
void msh_command_spezial_foreach(msh_info *, char Cond[], char text[]); // 476
void msh_command_spezial_forcount(msh_info *, char Cond[], char text[]); // 477
void msh_command_spezial_while(msh_info *, char Cond[], char text[]); // 478

// execute
// int msh_readZeile(char Zeile[], FUNC_LOCAL_STACK * stack); // 483
int msh_readZeile(msh_info * msh, const char * Zeile);
int msh_readScript(char Script[]); // 662
// int msh_readFunc(char Script[], FUNC_LOCAL_STACK * stack); // 676
int msh_readFunc(msh_info * msh, const char * Script, const char * funcName);
int msh_readFile(char filename[]); // 688

void msh_freeRessources();
// void msh_initRessources();

#ifdef __cplusplus
}
#endif

#endif