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
    ref.id = before->data.id + 1;
    SIMPLE_LIST_CREATE_EL(newP, ref, NULL);
    before->next = (indexP) newP;
    return &(newP->data);
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

    msh_ref * ref = msh_ref_insert_ref(msh, newRef);
    if (ref == NULL) {
        return -1;
    }
    return ref->id;
}

void msh_ref_remove(msh_info * msh, index32 id) {
    msh_ref_list temp = msh->refs;
    msh_ref_list before = NULL;
    SIMPLE_LIST_FOREACH(temp,
        if (temp->data.id == id) {
            indexP next = temp->next;
            msh_ref_free(&(temp->data));
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

word_picker msh_ref_get_data_as_string(msh_info * msh, index32 id) {
    msh_ref * res = msh_ref_get(msh, id);
    if (res == NULL) { return word_pick_from_to_index("<null>", 0, 6); }
    switch (res->type) {
        case msh_ref_type_BIN   : return word_pick_from_to_index("<binary data>", 0, 13); break;
        case msh_ref_type_STRING: return word_pick_from_to_index((const char *) res->data.pointer.cstr, 0, res->size); break;
        case msh_ref_type_SMARTSTRING : return word_pick_from_to("<use the read function instead>", "<", "\0");
        case msh_ref_type_FILE  : return word_pick_from_to_index("<file>", 0, 6); break;
    }
    return word_pick_from_to_index("<unknown>", 0, 9);
}

void msh_ref_read(msh_info * msh, index32 id, msh_ref_readHandler reader, void * userData) {
    msh_ref * res = msh_ref_get(msh, id);
    if (res == NULL) { return reader(userData, (const msh_val_char_t *) "<null>", 6); }
    switch (res->type) {
        case msh_ref_type_BIN    : reader(userData, (const msh_val_char_t *) "<binary data>", 13); return; break;
        case msh_ref_type_STRING : reader(userData, (const msh_val_char_t *) res->data.pointer.cstr, res->size); return; break;
        case msh_ref_type_SMARTSTRING : {
            const index32 len = sString_len(res->data.pointer.sstr);
            sString_read(res->data.pointer.sstr, reader, len, userData);
            return; break;
        }
        case msh_ref_type_FILE   : reader(userData, (const msh_val_char_t *) "<file>", 6); return; break;
    }
    return reader(userData, (const msh_val_char_t *) "<unknown>", 9);
}

void msh_ref_free(msh_ref * ref) {
    switch (ref->type) {
        case msh_ref_type_FILE : {
            fclose(ref->data.pointer.file);
            break;
        }
        case msh_ref_type_STRING : {
            MSH_FREE(ref->data.pointer.cstr);
            break;
        }
        case msh_ref_type_SMARTSTRING : {
            sString * buf = ref->data.pointer.sstr;
            if (buf != NULL) {
                sString_free(buf);
                MSH_FREE(buf);
                ref->data.pointer.general = NULL;
            }
            break;
        }
        case msh_ref_type_BIN : {
            break;
        }
    }
}

void msh_ref_freeAll(msh_info * msh) {
    msh_ref_list tempRefs = msh->refs;
    SIMPLE_LIST_FOREACH(tempRefs,
        msh_ref_free(&(tempRefs->data));
    )
    SIMPLE_LIST_FREE(msh->refs);
    msh->refs = NULL;
}

bool msh_ref_string_is_ref(const char * str) { return find(str, MSH_REF_STRING_SEPARATOR); }
word_picker_array msh_ref_string_split(const char * str) { return word_pick_split(str, MSH_REF_STRING_SEPARATOR); }

void msh_ref_resolve_str_reader(void * userData, const msh_val_char_t * data, sString_len_t len) {
    msh_info * msh = (msh_info *) userData;
    msh_val_add_static(msh, data, len);
}

void msh_ref_resolve_str(msh_info * msh, const char * str) {
    word_picker_array arr = msh_ref_string_split(str);
    if (arr.written == 0) { return; }
    msh_val_clear(msh);
    for (int i = 0; i < arr.written; i++) {
        word_picker w = WORD_PICKER_ARRAY_GET(arr, i);
        if (i % 2 /* == 1 */) { // each second is a reference - begins with 0
            index32 ref_id = msh_ref_id_from_string(w.begin, word_picker_len(w));
            msh_ref_read(msh, ref_id, msh_ref_resolve_str_reader, msh);
            continue;
        }
        msh_val_add(msh, (const msh_val_char_t *) w.begin, word_picker_len(w));
    }
    SIMPLE_ARRAY_FREE(arr);
}

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
        case msh_ref_type_SMARTSTRING: {
            sString * dest = ref->data.pointer.sstr;
            sString * src = data.pointer.sstr;
            sString_add(dest, *src);
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
            /*if (ref->allocated && len > ref->size) {
                ref->data.pointer = (indexP) MSH_REALLOC((void *) ref->data.pointer, len);
            }*/
            if (ref->data.pointer.cstr != NULL) { msh_ref_free(ref); }
            ref->data = data;
            ref->size = len;
            return;
            break;
        }
        case msh_ref_type_SMARTSTRING: {
            /*sString * dest = ref->data.pointer.sstr;
            const sString * src = data.pointer.sstr;
            if (dest == NULL) {
                ref->data.pointer.sstr = MSH_MALLOC(sizeof(sString));
                *dest = sString_create();
            }
            else {
                sString_free(dest);
                *dest = sString_create();
            }
            sString_add(dest, *src);*/
            if (ref->data.pointer.sstr != NULL) { sString_free(ref->data.pointer.sstr); }
            ref->data = data;
            return;
            break;
        }
        case msh_ref_type_FILE  : msh_error(msh, "msh_ref: unable to append to a file yet!"); return; break;
    }
    msh_error(msh, "msh_ref: unknown type of ref");
}
