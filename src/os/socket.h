#ifndef SOCKET_H
#define SOCKET_H

#include "base/types.h"

#include <stdbool.h>

typedef uptr Socket;

#define INVALID_SOCKET_HANDLE ((uptr)~(uptr)0)

struct sockaddr_in;

typedef struct SocketAddress SocketAddress; 
struct SocketAddress {
	u32 ip;
	u16 port;
};

bool 	socket_address_parse		(SocketAddress *out_address, const char *ip, u16 port);
Socket 	socket_create_udp			(void);
Socket 	socket_create_tcp			(void);
int 	socket_connect				(Socket socket, u32 server_ip, u16 server_port);
int 	socket_bind					(Socket socket, u16 port);
int 	socket_listen				(Socket socket);
Socket 	socket_accept				(Socket socket);
int 	socket_send					(Socket socket, const void *data, usize size);
int 	socket_send_to				(Socket socket, const void *data, usize size, const SocketAddress *address);
int 	socket_receive				(Socket socket, void *out, usize buf);
int 	socket_receive_from			(Socket socket, void *out, usize buf, SocketAddress *out_address);
void 	socket_close				(Socket socket);

void 	socket_cleanup				(void);

#endif