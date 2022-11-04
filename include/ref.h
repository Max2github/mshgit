#ifndef REF_H
#define REF_H

#include "../include/all_extern.h"

enum msh_ref_type {
    msh_ref_type_BIN,
    msh_ref_type_FILE,
    msh_ref_type_STRING,
};
typedef enum msh_ref_type msh_ref_type;

#define MSH_REF_TYPE_LAST msh_ref_type_STRING

struct msh_ref {
    index64 ref;                      // reference number
    enum msh_ref_type type : 1;           // type
    index64 size;

    index64 owner;                    // which thread, maybe even which function
    index64 data;                     // the pointer to the data or the data itself, if little
} ATTRIBUTE_PACKED;
typedef struct msh_ref msh_ref;

typedef SIMPLE_LIST(msh_ref) msh_ref_list;

void msh_ref_add(msh_info *, index64, void *, index64);
void msh_ref_remove(msh_info *, index64);

// index8 * msh_ref_getBin(msh_info *, index64);
// char * msh_ref_getString(msh_info *, index64);
void * msh_ref_get(msh_info *, index64);

#endif
