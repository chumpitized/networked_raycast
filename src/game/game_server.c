#include "game_server.h"

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

#include <ws2tcpip.h>

#include "os/time.h"

#include "networking/server.h"
#include "networking/client.h"

//we need to implement server replies to clients to validate movement...

void create_game_server(void *data) {
	if (!data) return;
	
	ServerConfig config = *(ServerConfig *)data;
	int port = config.port;

	uptr socket = socket_create_udp_server(port);

	if (socket == INVALID_SOCKET_HANDLE) {
		fprintf(stderr, "Game server creation failed.");
		return;
	}

	int buf_size = sizeof(PlayerMove);

	while (1) {
		double start = get_current_time();

		char buf[buf_size];
		int recv_resp = socket_receive(socket, buf, buf_size);		
		produce_parallel_queue(config.players_queue, &buf);

		double end = get_current_time();

		while (end - start < 0.033) {
			//need to make sure this is non-blocking...
			recv_resp = socket_receive(socket, buf, buf_size);		
			produce_parallel_queue(config.players_queue, &buf);
			
			end = get_current_time();
		}
	}
}

void create_game_client(void *data) {
	if (!data) return;

	ClientConfig config 		= *(ClientConfig *)data;
	EntityState *entity_state 	= config.entity_state;
	int entity_count 			= entity_state->count;

	int port = config.server_port;

	Socket socket = socket_create_udp_client(port);

	if (socket == INVALID_SOCKET_HANDLE) {
		fprintf(stderr, "Failed to create UDP client socket!\n");
		return;
	}

	printf("Entity count in client: %d\n", entity_count);

	double outside_start = get_current_time();

	while (1) {		
		double start = get_current_time();

		SocketAddress address;

		if (!socket_address_parse(&address, "127.0.0.1", 55555)) {
			fprintf(stderr, "Failed to get server address!\n");
		}

		for (int i = 0; i < entity_count; ++i) {
			PlayerMove move;
			move.player_id 	= i;
			move.sequence 	= 0;
			move.button 	= KEY_W;

			socket_send_to(socket, &move, sizeof(PlayerMove), &address);
		}

		double end = get_current_time();

		while (end - start < .033) {			
			end = get_current_time();
		};
	}

	double outside_end = get_current_time();
}