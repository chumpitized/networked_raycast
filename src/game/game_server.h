#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <stdbool.h>

#include "os/socket.h"

#include "parallel/parallel_queue.h"

#include "game/entity.h"

typedef struct Server Server;
struct Server {
	Socket 	socket;
	int 	port;
};

typedef struct Client Client;
struct Client {
	Socket 			socket;
	int 			port;
	SocketAddress 	client_service;
};

typedef struct PlayerMove PlayerMove;
struct PlayerMove {
	int player_id; //this is the entity  index for now...
	u32 sequence;
	u8 button; //e.g., KEY_W, KEY_A, KEY_S, KEY_D; if not one of these, ignore...


	/*int dx;
	int dy;*/
};

typedef struct PlayerPacket PlayerPacket;
struct PlayerPacket {
	PlayerMove move;
};

typedef struct ServerConfig ServerConfig;
struct ServerConfig {
	int port;
	ParallelQueue *players_queue;
};

typedef struct ClientConfig ClientConfig;
struct ClientConfig {
	int client_port;
	int server_port;
	EntityState *entity_state;
};

void create_game_server	(void *data);
void create_game_client	(void *data);

#endif