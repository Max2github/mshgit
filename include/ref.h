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

union msh_ref_data {
    index64 data;
    indexP pointer;
}; // 4 byte free i case of pointer, on 32-Bit systems

struct msh_ref {
    indexP ref;                 // reference number / string / whatever - some ID
    enum msh_ref_type type : 8; // type
    index64 size;

    index64 owner;              // which thread, maybe even which function
    union msh_ref_data data;  // the pointer to the data or the data itself, if little
} ATTRIBUTE_PACKED;
typedef struct msh_ref msh_ref;

typedef SIMPLE_LIST(msh_ref) msh_ref_list;

void msh_ref_add(msh_info *, indexP, union msh_ref_data, index64);
void msh_ref_remove(msh_info *, indexP);

// index8 * msh_ref_getBin(msh_info *, index64);
// char * msh_ref_getString(msh_info *, index64);
void * msh_ref_get(msh_info *, indexP);
void msh_ref_freeAll(msh_info *);

#endif
