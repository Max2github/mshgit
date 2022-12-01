#include "../../include/alg.h"

bool msh_exec_event_register(msh_info * msh) {
    MSH_MUTEX_LOCK(MSH_EXEC_EVENTS_MUTEX);

    msh_events_p neu;
    neu.stop = &(msh->event.stop);
    neu.exit = &(msh->event.exit);
    SIMPLE_LIST_ADDFIRST(MSH_EXEC_EVENTS, neu);

    MSH_MUTEX_UNLOCK(MSH_EXEC_EVENTS_MUTEX);

    return true;
}

bool msh_exec_event_unregister(msh_info * msh) {
    MSH_MUTEX_LOCK(MSH_EXEC_EVENTS_MUTEX);

    msh_events_p_list temp = MSH_EXEC_EVENTS;
    msh_events_p_list before = NULL;
    SIMPLE_LIST_FOREACH(temp,
        if (
            &(msh->event.exit) == temp->data.exit &&
            &(msh->event.stop) == temp->data.stop
        ) {
            unsigned long long next = temp->next;
            MSH_FREE(temp);
            if (before == NULL) { MSH_EXEC_EVENTS = (msh_events_p_list) next;}
            else { before->next = next; }
            break;
        }
    )

    MSH_MUTEX_UNLOCK(MSH_EXEC_EVENTS_MUTEX);

    return true;
}

void msh_exec_event_exitAll() {
    MSH_MUTEX_LOCK(MSH_EXEC_EVENTS_MUTEX);

    // remotely set event "exit" on every execution
    msh_events_p_list temp = MSH_EXEC_EVENTS;
    SIMPLE_LIST_FOREACH(temp,
        // if this execution is not terminating, terminate it
        if (!*(temp->data.exit)) {
            *(temp->data.exit) = true;
        }
    )

    MSH_MUTEX_UNLOCK(MSH_EXEC_EVENTS_MUTEX);

    // wait for list to be empty == wait for every execution to have terminated itself
    /* while(MSH_EXEC_EVENTS != NULL) {
        MSH_SLEEP(100);
    } */
}