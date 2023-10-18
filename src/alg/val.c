#include "../../include/alg.h"

void msh_val_init(msh_info * msh) { msh->val = sBuffer_create(0); };
void msh_val_clear(msh_info * msh) {
    //msh->val.written = 0;
    msh_val_free(msh);
    msh_val_init(msh);
}
void msh_val_free(msh_info * msh) { sBuffer_free(&(msh->val)); }

msh_val_len_t msh_val_count(msh_info * msh) { return sBuffer_count_single(&(msh->val)); };
msh_val_len_t msh_val_len(msh_info * msh) { return sBuffer_count(&(msh->val)); };

msh_val_len_t msh_val_read(msh_info * msh, msh_val_pos_t start, msh_val_reader reader, msh_val_len_t length, void * userData) {
    msh_val_reader_data data = (msh_val_reader_data) { msh, userData };
    return sBuffer_read(&(msh->val), (sBuffer_readHandler) reader, length, &data);
}

// writing
msh_val_len_t msh_val_write(msh_info * msh, msh_val_pos_t start, const msh_val_char_t * data, msh_val_len_t length) {
    msh_val_len_t index = 0;
    sBuffer_single_ptr temp = sBuffer_get(&(msh->val), index);
    for (; temp != NULL;) {
        msh_val_len_t count = sBuffer_single_count(temp);
        start -= count;
        if (start <= 0) {
            break;
        }
        // continue / next element
        index++;
        temp = sBuffer_get(&(msh->val), index);
    }
    start = start * -1;
    if (temp == NULL) {
        return 0;
    } else if (start == 0) {
        sBuffer_addStr(&(msh->val), data, length);
        return sBuffer_get(&(msh->val), index)->data.written;
    } else {
        SIMPLE_ARRAY_WRITE(temp->data, start, data, length);
        return length;
    }
    return 0;
}
void msh_val_add(msh_info * msh, const msh_val_char_t * data, msh_val_len_t length) { return sBuffer_addStr(&(msh->val), data, length); }
void msh_val_add_static(msh_info * msh, const msh_val_char_t * data, msh_val_len_t length) {
    return sBuffer_add(&(msh->val), sBuffer_single_create_static(data, length));
};
