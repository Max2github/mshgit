#pragma once
#include "../dependencies/list.h"

#ifdef __cplusplus
extern "C" {
#endif

list msh_getListByName(const char * name);
list msh_getListBossByName(const char * name);
list msh_getNodeByName(const char * name);
list msh_getBeforeNodeByName(const char * name);

#ifdef __cplusplus
}
#endif