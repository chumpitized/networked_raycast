#ifndef SETUP_H
#define SETUP_H

#include "os/thread.h"

#include "parallel/thread_pool.h"
#include "parallel/parallel_queue.h"

#include "game/player.h"
#include "game/ray.h"
#include "game/world.h"
#include "game/ray.h"
#include "game/entity.h"
#include "game/game_server.h"

#include "render/assets.h"
#include "render/draw.h"
#include "render/render.h"

typedef struct AppContext AppContext;
struct AppContext {
	Player 					player;
	World					world;
	RaycastResult			raycast_result;
	Assets					assets;
	EntityState				entity_state;
	MinimapRenderContext 	minimap_context;
	RenderContext			render_context;
	FrameRenderContext		frame_render_context;
	ThreadPool				thread_pool;
	ParallelQueue 			network_queue;
	ClientConfig			client_config;
	ServerConfig			server_config;
};

void setup(AppContext *context);

#endif