#ifndef CLIENT_H
#define CLIENT_H

#include "os/socket.h"

Socket 	socket_create_udp_client	(u16 port);
Socket 	socket_create_tcp_client	(u16 port);

#endif