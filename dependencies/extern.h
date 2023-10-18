#ifndef MSH_DEP_EXTERN_H
#define MSH_DEP_EXTERN_H

/*#include <Max2/templates/def.h>
#include <Max2/templates/simple_array.h>
#include <Max2/templates/simple_list.h>
#include <Max2/words_h.h>
#include <Max2/list_h.h>*/

/* #define MSH_LIB_DIR "../../../lib/"
#define MSH_INCLUDE_TEMPLATES_DEF_H MSH_LIB_DIR##"templates/def.h"
#include MSH_INCLUDE_TEMPLATES_DEF_H
#include MSH_LIB_DIR "templates/simple_array.h"
#include MSH_LIB_DIR "templates/simple_list.h"*/

#include "all_defines.h"

#include "../../../lib/templates/def.h"
#include "../../../lib/templates/simple_array.h"
#include "../../../lib/templates/simple_list.h"

#include "../../../lib/freestanding/general_alloc.h"

#if 0
#include "../../../lib/smartbuffer.h"
#else
#include "../../../lib/smartstring.h"
#endif

#include "../../../lib/words_h.h"
#include "../../../lib/list_h.h"
#include "../../../lib/word_pick_h.h"

// #include "../../../lib/mathe_h.h"
#include "mathe.h"
// #include "../../../lib/superstring_h.h"
#include "superstring.h"

#endif
