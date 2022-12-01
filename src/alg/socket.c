#include "../../include/alg.h"

#if MSH_ALLOW_SOCKET

#if MSH_ALLOW_MULTI_THREAD
    MSH_MUTEX MSH_SOCKET_MUTEX = MSH_MUTEX_DEFAULT;
#endif

MSH_SOCKET msh_socket_create(int ipVersion, int socketType, int subProto) {
    MSH_SOCKET ret;

    #if DEF_PF_UNIX
        ret = socket(ipVersion, socketType, subProto);
    #elif DEF_PF_WINDOWS

    #endif

    msh_socket_register(ret);

    return ret;
}

void msh_socket_register(MSH_SOCKET sock) {
    MSH_MUTEX_LOCK(MSH_SOCKET_MUTEX);

    SIMPLE_LIST_ADDFIRST(MSH_SOCKET_ALL, sock);

    MSH_MUTEX_UNLOCK(MSH_SOCKET_MUTEX);
}

bool msh_socket_compare(MSH_SOCKET sock1, MSH_SOCKET sock2) {
    #if DEF_PF_UNIX
        if (
            sock1 == sock2
        ) {
            return true;
        }
    #elif DEF_PF_WINDOWS

    #endif
    return false;
}

void msh_socket_unregister(MSH_SOCKET sock) {
    MSH_MUTEX_LOCK(MSH_SOCKET_MUTEX);

    msh_socket_list temp = MSH_SOCKET_ALL;
    msh_socket_list before = NULL;
    SIMPLE_LIST_FOREACH(temp, 
        if (msh_socket_compare(temp->data, sock)) {
            unsigned long long next = temp->next;
            MSH_FREE(temp);
            if (before == NULL) { MSH_SOCKET_ALL = (msh_socket_list) next; }
            else { before->next = next; }
            break;
        }
        before = (msh_socket_list) temp->next;
    )

    MSH_MUTEX_UNLOCK(MSH_SOCKET_MUTEX);
}

void msh_socket_close(MSH_SOCKET sock) {
    msh_socket_unregister(sock);
    #if DEF_PF_UNIX
        close(sock);
        close(sock);
    #elif DEF_PF_WINDOWS

    #endif
}

void msh_socket_closeAll() {
    MSH_MUTEX_LOCK(MSH_SOCKET_MUTEX);

    msh_socket_list temp = MSH_SOCKET_ALL;
    SIMPLE_LIST_FOREACH(temp,
        msh_socket_close(temp->data);
    )
    SIMPLE_LIST_FREE(MSH_SOCKET_ALL);
    MSH_SOCKET_ALL = NULL;

    MSH_MUTEX_UNLOCK(MSH_SOCKET_MUTEX);
}

#endif