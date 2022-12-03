#include "../include/ref.h"
#include "../include/alg.h"
//#include "../dependencies/all.c"

void msh_ref_add(msh_info * msh, indexP ref, union msh_ref_data data, index64 nbytes) {
    msh_ref newRef;
    newRef.data = data;
    newRef.size = nbytes;
    newRef.ref = ref;

    // fill later correctly
    newRef.owner = 1;
    newRef.type = msh_ref_type_BIN;
    
    SIMPLE_LIST_ADDFIRST(msh->refs, newRef);
}

void msh_ref_remove(msh_info * msh, indexP ref) {
    msh_ref_list temp = msh->refs;
    msh_ref_list before = NULL;
    SIMPLE_LIST_FOREACH(temp,
        // if (temp->data.ref == ref) {
        if (s_compare((superstring) temp->data.ref, (superstring) ref)) {
            indexP next = temp->next;
            s_free((superstring) temp->data.ref);
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

void * msh_ref_get(msh_info * msh, indexP ref) {
    msh_ref_list temp = msh->refs;
    if (temp == NULL) { return NULL; } // maybe send an error?
    SIMPLE_LIST_FOREACH(temp,
        // if (temp->data.ref == ref) { return (void *) temp->data.data; }
        if (s_compare((superstring) temp->data.ref, (superstring) ref) == 0) { return (void *) temp->data.data.pointer; }
    )
    return NULL;
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
        s_free((superstring) msh->refs->data.ref);
    )
    SIMPLE_LIST_FREE(msh->refs);
    msh->refs = NULL;
}