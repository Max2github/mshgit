#include "../include/ref.h"
#include "../include/alg.h"

msh_ref * msh_ref_insert_ref(msh_info * msh, msh_ref ref) {
    if (msh->refs == NULL){
        ref.id = 0;
        SIMPLE_LIST_ADDFIRST(msh->refs, ref);
        return &(msh->refs->data);
    }
    msh_ref_list temp = msh->refs;
    msh_ref_list before = temp;
    SIMPLE_LIST_FOREACH(temp,
        index32 id = temp->data.id;
        // if there is an empty place between before and now
        if (id > before->data.id + 1) {
            // set the id
            ref.id = id - 1;
            // insert the ref
            void * next = (void *) temp->next;
            SIMPLE_LIST_CREATE_EL(temp, ref, next);
            return &(temp->data);
        }
        before = temp;
    );
    // if there was no empty space in between, append it at the end
    msh_ref_list newP = NULL; // newP will be assigned the allocated address
    SIMPLE_LIST_CREATE_EL(newP, ref, NULL);
    before->next = (indexP) newP;

    return NULL;
}

const index32 msh_ref_string_separator_len() { return word_len(MSH_REF_STRING_SEPARATOR); }
const index32 msh_ref_id_as_string_len(index32 id) { return msh_ref_string_separator_len() * 2 + intLen(id) + 1; }

index32 msh_ref_id_from_string(const char * str, index32 len) {
    word_picker w = word_pick_until_index(str, len);
    if (word_picker_find(w, MSH_REF_STRING_SEPARATOR)) {
        w.begin = word_seek_after(str, MSH_REF_STRING_SEPARATOR);
        w.end = word_seek(w.begin, MSH_REF_STRING_SEPARATOR);
    }
    // like MSH_STRING_TO_INT, but for word_picker
    index32 id = 0;
    for(; w.begin != w.end; w.begin++) {
        id = id * 10 + (*(w.begin) - 48); // '0' == 48
    }
    return id;
}

void msh_ref_id_to_string(index32 id, char * saveto) {
    word_copy(saveto, MSH_REF_STRING_SEPARATOR);
    intToString(id, saveto + msh_ref_string_separator_len());
    word_add(saveto, MSH_REF_STRING_SEPARATOR);
}

index32 msh_ref_add(msh_info * msh, union msh_ref_data data, msh_ref_type type, index64 nbytes) {
    msh_ref newRef;
    newRef.data = data;
    newRef.size = nbytes;

    // fill later correctly
    newRef.owner = 1;
    newRef.type = type;

    return msh_ref_insert_ref(msh, newRef)->id;
}

void msh_ref_remove(msh_info * msh, index32 id) {
    msh_ref_list temp = msh->refs;
    msh_ref_list before = NULL;
    SIMPLE_LIST_FOREACH(temp,
        if (temp->data.id == id) {
            indexP next = temp->next;
            MSH_FREE(temp);
            if (before == NULL) {
                msh->refs = (msh_ref_list) next;
                return;
            }
            before->next = next;
            return;
        }
        before = temp;
    )
}

msh_ref * msh_ref_get(msh_info * msh, index32 id) {
    msh_ref_list temp = msh->refs;
    if (temp == NULL) { return NULL; } // maybe send an error?
    SIMPLE_LIST_FOREACH(temp,
        if (temp->data.id == id) { return &(temp->data); }
    )
    return NULL;
}

union msh_ref_data msh_ref_get_data(msh_info * msh, index32 id) {
    union msh_ref_data bad; bad.data = 0;
    msh_ref * res = msh_ref_get(msh, id);
    if (res == NULL) { return bad; }
    return res->data;
}

const char * msh_ref_get_data_as_string(msh_info * msh, index32 id) {
    msh_ref * res = msh_ref_get(msh, id);
    if (res == NULL) { return "<null>"; }
    switch (res->type) {
        case msh_ref_type_BIN   : return "<binary data>"; break;
        case msh_ref_type_STRING: return (const char *) res->data.pointer; break;
        case msh_ref_type_FILE  : return "<file>"; break;
    }
    return "<unknown>";
}

void msh_ref_freeAll(msh_info * msh) {
    msh_ref_list tempRefs = msh->refs;
    SIMPLE_LIST_FOREACH(tempRefs,
        switch (tempRefs->data.type) {
            case msh_ref_type_FILE : {
                fclose((FILE *) tempRefs->data.data.pointer);
                break;
            }
            case msh_ref_type_STRING : {
                break;
            }
            case msh_ref_type_BIN : {
                break;
            }
        }
    )
    SIMPLE_LIST_FREE(msh->refs);
    msh->refs = NULL;
}

bool msh_ref_string_is_ref(const char * str) { return find(str, MSH_REF_STRING_SEPARATOR); }
word_picker_array msh_ref_string_split(const char * str) { return word_pick_split(str, MSH_REF_STRING_SEPARATOR); }

void msh_ref_append(msh_info * msh, index32 id, union msh_ref_data data, index64 len) {
    msh_ref * ref = msh_ref_get(msh, id);
    switch (ref->type) {
        case msh_ref_type_BIN   : msh_error(msh, "msh_ref: unable to append to a binary yet!"); return; break;
        case msh_ref_type_STRING: {
            // memory management?
            /*char * writeTo = ((char *) ref->data.pointer) + ref->size;
            memory_copy((void *) writeTo, (void *) data.pointer, len);*/
            msh_error(msh, "msh_ref: unable to append to a string yet!");
            return;
            break;
        }
        case msh_ref_type_FILE  : msh_error(msh, "msh_ref: unable to append to a file yet!"); return; break;
    }
    msh_error(msh, "msh_ref: unknown type of ref");
}
void msh_ref_assign(msh_info * msh, index32 id, union msh_ref_data data, index64 len) {
        msh_ref * ref = msh_ref_get(msh, id);
    switch (ref->type) {
        case msh_ref_type_BIN   : msh_error(msh, "msh_ref: unable to append to binary yet!"); return; break;
        case msh_ref_type_STRING: {
            /*if (len > ref->size) {
                ref->data.pointer = (indexP) MSH_REALLOC((void *) ref->data.pointer, len);
            }*/
            ref->data = data;
            return;
            break;
        }
        case msh_ref_type_FILE  : msh_error(msh, "msh_ref: unable to append to a file yet!"); return; break;
    }
    msh_error(msh, "msh_ref: unknown type of ref");
}
