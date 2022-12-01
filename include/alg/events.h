#ifndef MSH_ALG_EVENTS_H
#define MSH_ALG_EVENTS_H

#include "../../dependencies/std.h"

#include "../../include/all_extern.h"

/**
 * Set simple (!!!) events on msh_info remotely.
 */
struct msh_events_p {
    bool * stop;
    bool * exit;
} ATTRIBUTE_PACKED; // 16 bytes (without packed) 0 bytes free
typedef struct msh_events_p msh_events_p;
typedef SIMPLE_LIST(msh_events_p) msh_events_p_list;

bool msh_exec_event_register(msh_info *);
bool msh_exec_event_unregister(msh_info *);
void msh_exec_event_exitAll();

#endif