#include "server.h"

Socket socket_create_udp_server(u16 port) {
	Socket socket = socket_create_udp();
	if (socket == INVALID_SOCKET_HANDLE) {
		return INVALID_SOCKET_HANDLE;
	}

	if (socket_bind(socket, port) < 0) {
		socket_close(socket);
		return INVALID_SOCKET_HANDLE;
	}

	return socket;
}

Socket socket_create_tcp_server(u16 port) {
	Socket socket = socket_create_tcp();
	if (socket == INVALID_SOCKET_HANDLE) {
		return INVALID_SOCKET_HANDLE;
	}

	if (socket_bind(socket, port) < 0) {
		socket_close(socket);
		return INVALID_SOCKET_HANDLE;
	}

	if (socket_listen(socket) < 0) {
		socket_close(socket);
		return INVALID_SOCKET_HANDLE;
	}

	return socket;
}