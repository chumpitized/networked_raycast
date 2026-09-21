#ifndef SERVER_H
#define SERVER_H

#include "os/socket.h"

Socket socket_create_udp_server	(u16 port);
Socket socket_create_tcp_server	(u16 port);

#endif