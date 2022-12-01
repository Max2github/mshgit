#include "../../dependencies/std.h"

#if !defined(MSH_ALG_SOCKET_H) && MSH_ALLOW_SOCKET
#define MSH_ALG_SOCKET_H

#if MSH_ALLOW_MULTI_THREAD
    extern MSH_MUTEX MSH_SOCKET_MUTEX;
#endif

/**
 * register all socket connections
 */
typedef SIMPLE_LIST(MSH_SOCKET) msh_socket_list;

#define MSH_SOCKET_TCP_SOCKET SOCK_STREAM
#define MSH_SOCKET_UDP_SOCKET SOCK_DGRAM
#define MSH_SOCKET_IPV4 AF_INET
#define MSH_SOCKET_IPV6 AF_INET6
#define MSH_SOCKET_PROTOCOLL_IP IPPROTO_IP

MSH_SOCKET msh_socket_create(int ipVersion, int socketType, int subProto);
void msh_socket_register(MSH_SOCKET);
bool msh_socket_compare(MSH_SOCKET, MSH_SOCKET);
void msh_socket_unregister(MSH_SOCKET sock);
void msh_socket_close(MSH_SOCKET sock);
void msh_socket_closeAll();

#endif