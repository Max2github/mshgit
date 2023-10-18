#ifndef MSH_ALG_VAL_H
#define MSH_ALG_VAL_H

#include "../all_extern.h"

#include "../../dependencies/extern.h"

typedef sBuffer msh_val;
typedef SMARTBUFFER_CHAR msh_val_char_t;
typedef SMARTBUFFER_LEN_T msh_val_pos_t;
typedef SMARTBUFFER_LEN_T msh_val_len_t;

typedef struct { msh_info * msh; void * userData; } msh_val_reader_data;
// typedef sBuffer_readHandler msh_val_reader;
/**
 * @brief A reader funtion pointer used for msh_val_read
 * @param 0 (msh_val_reader_data *) msh_info + the user data, which was passed to msh_val_read
 * @param 1 (const msh_val_char_t *) the data to read
 * @param 2 (msh_val_len_t) how long the data to read is
 */
typedef void (*msh_val_reader)(msh_val_reader_data *, const msh_val_char_t *, msh_val_len_t);

#define MSH_VAL_EMPTY (sBuffer) { 0, 0, 0 }

void msh_val_init(msh_info *);
/**
 * @brief Clear the buffer
 * This will not any free allocated resources!!!
 */
void msh_val_clear(msh_info *);
/**
 * @brief Free the buffer
 * All allocated resources will be freed (and the buffer cleared).
 */
void msh_val_free(msh_info *);

msh_val_len_t msh_val_count(msh_info *);
msh_val_len_t msh_val_len(msh_info *);

// reading
msh_val_len_t msh_val_read(msh_info *, msh_val_pos_t, msh_val_reader, msh_val_len_t, void *);

// writing
msh_val_len_t msh_val_write(msh_info *, msh_val_pos_t, const msh_val_char_t *, msh_val_len_t);
void msh_val_add(msh_info *, const msh_val_char_t *, index64);
void msh_val_add_static(msh_info *, const msh_val_char_t *, index64);

#endif
