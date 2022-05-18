#pragma once
#include "../dependencies/list.h"

#ifdef __cplusplus
extern "C" {
#endif

// msh-functions
int msh_push_Var(char value[], char name[]); // 62
int msh_get_Var(char name[], char saveto[]); // 86
int msh_get_Var_element(int Var_index, char el_name[], char saveto[]); // 113
int msh_fillVar(char Code[]); // 126
int msh_fillObj(char Code[]); // 144
int msh_Breaks(char Code[]); // 197

// Commands
// main
void msh_command_main_display(); // 212
void msh_command_main_print(); // 223
void msh_command_main_stop(); // 233
void msh_command_main_del(); // 236
void msh_command_main_call(); // 253
void msh_command_main_goto(); // 256
    // list (part of main)
    list msh_getListByName(const char * name);
    list msh_getNodeByName(const char * name);
    list msh_getBeforeNodeByName(const char * name);
    void msh_command_main_list_add();
    void msh_command_main_list_remove();
    void msh_command_main_list_mod();
    void msh_command_main_list_print();
    void msh_command_main_list_delete();
    void msh_command_main_list_print_SPEICHER();
// sub
    // list
    void msh_command_sub_list_toString();
    void msh_command_sub_list_getFirst();
    void msh_command_sub_list_len();
void msh_command_sub_pointto(); // 260
void msh_command_sub_array(); 
void msh_command_sub_object(); // 287
void msh_command_sub_random(); // 296
void msh_command_sub_randomin(); // 311
void msh_command_sub_input(); // 321
void msh_command_sub_math(); // 328
void msh_command_sub_row(); // 346
void msh_command_sub_ascii(); // 349
    // list
    void msh_command_sub_list_getFirst();
// spezial
int msh_command_isSpezial(char Code[]); // 355
void msh_command_spezial_def(char Cond[], char text[]); // 373
void msh_command_spezial_if(char Cond[], char text[]); // 376
void msh_command_spezial_for(char Cond[], char text[]); // 448
void msh_command_spezial_foreach(char Cond[], char text[]); // 476
void msh_command_spezial_forcount(char Cond[], char text[]); // 477
void msh_command_spezial_while(char Cond[], char text[]); // 478

// execute
int msh_readZeile(char Zeile[]); // 483
int msh_readScript(char Script[]); // 662
int msh_readFunc(char Script[]); // 676
int msh_readFile(char filename[]); // 688

#ifdef __cplusplus
}
#endif