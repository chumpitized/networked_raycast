#include "client.h"

#include <ws2tcpip.h>

Socket socket_create_udp_client(u16 port) {
	Socket socket = socket_create_udp();

	if (socket == INVALID_SOCKET_HANDLE) {
		return INVALID_SOCKET_HANDLE;
	}

	return socket;
}

Socket socket_create_tcp_client(u16 port) {
	return INVALID_SOCKET_HANDLE;
}