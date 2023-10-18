#ifndef REF_H
#define REF_H

#include "all_extern.h"
#include "../dependencies/extern.h"

enum msh_ref_type {
    msh_ref_type_BIN,
    msh_ref_type_FILE,
    msh_ref_type_STRING,
    msh_ref_type_SMARTSTRING
};
typedef enum msh_ref_type msh_ref_type;

#define MSH_REF_TYPE_LAST msh_ref_type_SMARTBUFFER_STRING

union msh_ref_data_pointer {
    void * general;
    char * cstr;
    //sBuffer * buf;
    sString * sstr;
    FILE * file;
};

union msh_ref_data {
    index64 data;
    union msh_ref_data_pointer pointer;
}; // 4 byte free in case of pointer, on 32-Bit systems

// may add:
// - bool for const / read only - not writable / mutable
// - may add separation between allocated and written (like in SIMPLE_ARRAY)
struct msh_ref {
    index32 id;            // reference number / string / whatever - some ID
    msh_ref_type type : 8; // type
    index64 size;

    index32 owner;            // which thread, maybe even which function
    union msh_ref_data data;  // the pointer to the data or the data itself, if little
};
typedef struct msh_ref msh_ref;

typedef SIMPLE_LIST(msh_ref) msh_ref_list;

#define MSH_REF_STRING_SEPARATOR "&/ref//"

msh_ref * msh_ref_insert_ref(msh_info *, msh_ref);

const index32 msh_ref_string_separator_len();
const index32 msh_ref_id_as_string_len(index32 id);
index32 msh_ref_id_from_string(const char *, index32);
void msh_ref_id_to_string(index32, char *);

index32 msh_ref_add(msh_info *, union msh_ref_data, msh_ref_type, index64);
void msh_ref_remove(msh_info *, index32);

//typedef void(*msh_ref_reader)(word_picker, void *);
typedef sString_readHandler msh_ref_readHandler;
void msh_ref_read(msh_info * msh, index32 id, msh_ref_readHandler reader, void * userData);

// index8 * msh_ref_getBin(msh_info *, index64);
// char * msh_ref_getString(msh_info *, index64);
msh_ref * msh_ref_get(msh_info *, index32);
union msh_ref_data msh_ref_get_data(msh_info *, index32);
word_picker msh_ref_get_data_as_string(msh_info *, index32);

void msh_ref_free(msh_ref * ref);
void msh_ref_freeAll(msh_info *);

bool msh_ref_string_is_ref(const char * str);
word_picker_array msh_ref_string_split(const char * str);
void msh_ref_resolve_str(msh_info * msh, const char * str);

void msh_ref_append(msh_info *, index32, union msh_ref_data, index64);
void msh_ref_assign(msh_info *, index32, union msh_ref_data, index64);

#endif
