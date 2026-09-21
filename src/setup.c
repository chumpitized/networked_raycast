#include "setup.h"

void setup(AppContext *context) {
	Player 					*player 				= &context->player;
	World 					*world 					= &context->world;
	RaycastResult 			*raycast_result 		= &context->raycast_result;
	Assets 					*assets					= &context->assets;
	EntityState				*entity_state 			= &context->entity_state;
	MinimapRenderContext 	*minimap_context 		= &context->minimap_context;
	RenderContext 			*render_context			= &context->render_context;
	FrameRenderContext		*frame_render_context 	= &context->frame_render_context;
	ThreadPool 				*thread_pool			= &context->thread_pool;
	ParallelQueue 			*network_queue			= &context->network_queue;
	ClientConfig			*client_config			= &context->client_config;
	ServerConfig			*server_config			= &context->server_config;

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Raycast");
	DisableCursor();

	ClearBackground(RAYWHITE);

	RenderTexture2D minimap_texture2d = LoadRenderTexture(RENDER_MINIMAP_WIDTH, RENDER_MINIMAP_HEIGHT);
	BeginTextureMode(minimap_texture2d);
	ClearBackground(RAYWHITE);
	EndTextureMode();

	player_init_default(player);
	world_init(world);

	if (!assets_load(assets)) {
		CloseWindow();
		fprintf(stderr, "Loading assets failed!");
		abort();
	}
	
	init_entity_state(entity_state, 1000);

	minimap_context->texture 		= minimap_texture2d;
	minimap_context->raycast_result = raycast_result;
	minimap_context->world 			= world;
	minimap_context->entity_state	= entity_state;
	minimap_context->assets 		= assets;
	minimap_context->player 		= player;

	draw_minimap_to_texture(minimap_context);

	render_context->texture 		= minimap_texture2d;
	render_context->raycast_result 	= raycast_result;
	render_context->assets			= assets;
	render_context->world 			= world;
	render_context->entity_state	= entity_state;
	render_context->player 			= player;
	render_context->depth_view		= false;

	frame_render_context->player 					= player;
	frame_render_context->raycast_result 			= raycast_result;
	frame_render_context->minimap_render_context 	= minimap_context;
	frame_render_context->entity_state				= entity_state;
	frame_render_context->render_context 			= render_context;
	frame_render_context->thread_pool				= thread_pool;

	//-1 if we want the main thread excluded in the pool
	//could do -3 because we need 1 thread each for server and client
	int cpu_thread_count = get_cpu_thread_count();
	if (cpu_thread_count < 1) {
		assert(0 && "CPU Thread Count is zero!");
	}

	init_parallel_queue(network_queue, sizeof(PlayerMove), 1 << 10);

	server_config->port = 55555;
	server_config->players_queue = network_queue;

	client_config->client_port 	= 55551;
	client_config->server_port 	= 55555;
	client_config->entity_state	= entity_state;

	//we probably do want to keep the main thread out of the threadpool...
	init_thread_pool(thread_pool, cpu_thread_count, 1 << 10);
	enqueue_thread_pool(thread_pool, create_game_server, server_config);
	enqueue_thread_pool(thread_pool, create_game_client, client_config);
}