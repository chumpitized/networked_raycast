#include "socket.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef NOGDI
#define NOGDI
#endif

#ifndef NOUSER
#define NOUSER
#endif

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <limits.h>

//file-scoped flag for winsock initialization
static bool socket_initialized = false;

bool socket_address_parse(SocketAddress *out_address, const char *ip, u16 port) {
	if (!out_address || !ip) {
      return false;
	}

  	IN_ADDR addr = {0};
  	if (InetPtonA(AF_INET, ip, &addr) != 1) {
  	    return false;
  	}

  	out_address->ip = ntohl(addr.s_addr);
  	out_address->port = port;

	return true;
}

static struct sockaddr_in socket_address_to_win32(SocketAddress address) {
	struct sockaddr_in result = {0};
	
	result.sin_family = AF_INET;
	result.sin_port = htons(address.port);
	result.sin_addr.s_addr = htonl(address.ip);

	return result;
}

static SocketAddress socket_address_from_win32(struct sockaddr_in win32_address) {
	SocketAddress result = {0};
	
	result.ip = ntohl(win32_address.sin_addr.s_addr);
	result.port = ntohs(win32_address.sin_port);

	return result;
}

static bool socket_init(void) {
	if (!socket_initialized) {
		WSADATA wsa_data;
		WORD winsock_version 	= MAKEWORD(2, 2);
		int wsa_err 			= WSAStartup(winsock_version, &wsa_data);

		if (wsa_err != 0) {
			fprintf(stderr, "WSAStartup failed with error: %d\n", wsa_err);
			return false;
		}
		
		socket_initialized = true;
	}

	return true;
}

//add blocking flag?
Socket socket_create_udp(void) {
	if (!socket_init()) {
		return INVALID_SOCKET_HANDLE;
	}
	
	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (s == INVALID_SOCKET_HANDLE) {
		int err = WSAGetLastError();
		fprintf(stderr, "Socket creation failed with error: %d\n", err);
		return INVALID_SOCKET_HANDLE;
	}

	/*u_long i_mode = 1;
		if (ioctlsocket(s, FIONBIO, &i_mode) != 0) {
		int err = WSAGetLastError();
		fprintf(stderr, "Failed to make socket non-blocking: %d\n", err);
		closesocket(s);
		return INVALID_SOCKET_HANDLE;
	}*/

	return (Socket)s;
}

Socket socket_create_tcp(void) {
	if (!socket_init()) {
		return INVALID_SOCKET_HANDLE;
	}
	
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (s == INVALID_SOCKET_HANDLE) {
		int err = WSAGetLastError();
		fprintf(stderr, "Socket creation failed with error: %d\n", err);
		return INVALID_SOCKET_HANDLE;
	}

	/*u_long i_mode = 1;
		if (ioctlsocket(s, FIONBIO, &i_mode) != 0) {
		int err = WSAGetLastError();
		fprintf(stderr, "Failed to make socket non-blocking: %d\n", err);
		closesocket(s);
		return INVALID_SOCKET_HANDLE;
	}*/

	return (Socket)s;
}

int socket_connect(uptr socket, u32 server_ip, u16 server_port) {
	struct sockaddr_in server;
	server.sin_family 		= AF_INET;
	server.sin_port 		= htons(server_port);
	server.sin_addr.s_addr 	= htonl(server_ip);

	if (connect(socket, (struct sockaddr *)&server, sizeof(server)) == -1) {
		int err = WSAGetLastError();
		fprintf(stderr, "Socket failed to connect to server with error: %d\n", err);
		return -1;
	}

	return 0;
}

int socket_bind(uptr socket, u16 port) {
	struct sockaddr_in server;
	server.sin_family 		= AF_INET;
	server.sin_port 		= htons(port);
	server.sin_addr.s_addr 	= INADDR_ANY;

	if (bind(socket, (struct sockaddr *)&server, sizeof(server)) == -1) {
		int err = WSAGetLastError();
		fprintf(stderr, "Socket failed to bind with error: %d\n", err);
		return -1;
	}

	return 0;
}

int socket_listen(Socket socket) {
	if (listen(socket, 1) == -1) {
		int err = WSAGetLastError();
		fprintf(stderr, "Socket failed to listen with error: %d\n", err);
		return -1;
	}

	return 0;
}

Socket socket_accept(Socket socket) {
	uptr client_socket = accept(socket, NULL, NULL);

	if (client_socket == INVALID_SOCKET_HANDLE) {
		int err = WSAGetLastError();
		fprintf(stderr, "Socket failed to accept with error: %d\n", err);
		return INVALID_SOCKET_HANDLE;
	}

	return client_socket;
}

int socket_send(Socket socket, const void *data, usize size) {
	int result = send(socket, data, (int)size, 0);
	
	if (result == SOCKET_ERROR) {
		int err = WSAGetLastError();
		fprintf(stderr, "Socket failed to send with error: %d\n", err);
		return -1;
	}

	return result;
}

int socket_send_to(Socket socket, const void *data, usize size, const SocketAddress *address) {
	if (!address) return -1;

	struct sockaddr_in to = socket_address_to_win32(*address);

	int result = sendto(socket, data, size, 0, (struct sockaddr *)&to, sizeof(to));

	if (result == -1) {
		int err = WSAGetLastError();
		fprintf(stderr, "Socket failed to sendto with error: %d\n", err);
		return -1;
	}

	return result;
}

int socket_receive(Socket socket, void *out, usize buf) {
	int result = recv(socket, out, buf, 0);

	if (result == SOCKET_ERROR) {
		int err = WSAGetLastError();
		fprintf(stderr, "Socket failed to recv with error: %d\n", err);
		return -1;
	}

	//if 0 in TCP, client has closed sending connection
	//if 0 in UDP, could be 0-byte datagram
	return result;
}

//presently, the address is optional... 
//meaning we can return the bytes but not the address...
//we may want to require it...
int socket_receive_from(Socket socket, void *out, usize size, SocketAddress *out_address) {
	if (size > INT_MAX) {
		return SOCKET_ERROR;
	}

	struct sockaddr_in from = {0};
	int from_size = sizeof(from);

	int result = recvfrom(socket, out, (int)size, 0, (struct sockaddr *)&from, &from_size);

	if (result == SOCKET_ERROR) {
		int err = WSAGetLastError();
		fprintf(stderr, "Socket failed to recvfrom with error: %d\n", err);
		return -1;
	}

	if (out_address) {
		*out_address = socket_address_from_win32(from);
	}

	return result;
}

void socket_close(Socket socket) {
	closesocket(socket);
}

void socket_shutdown(void) {
	if (socket_initialized) {
		WSACleanup();
		socket_initialized = false;
	}
}