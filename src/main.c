#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include "os/time.h"
#include "os/thread.h"

#include "parallel/atomics.h"
#include "parallel/thread_pool.h"
#include "parallel/job_counter.h"
#include "parallel/parallel_for.h"

#include "game/world.h"
#include "game/entity.h"
#include "game/npc_movement.h"
#include "game/simulate.h"

#include "networking/server.h"

#include "render/draw.h"
#include "render/assets.h"
#include "render/render.h"

#include "setup.h"

#include "raylib.h"

int main(void) {
	SetTargetFPS(60);

	AppContext app_context = {0};	
	setup(&app_context);

	double sim_tick_rate	= 1.0 / 60.0;
	double sim_accumulator 	= 0.0;

	while (!WindowShouldClose()) {
		//////////////
		// Simulate //
		//////////////
		double delta_time 	= GetFrameTime();
		sim_accumulator    += delta_time;

		while (sim_accumulator >= sim_tick_rate) {
			simulate(&app_context.network_queue, &app_context.entity_state, sim_tick_rate);
			sim_accumulator -= sim_tick_rate;
		}

		///////////
		// Input //
		///////////
		player_input(&app_context.player);
		if (IsKeyPressed(KEY_P)) {
			app_context.render_context.depth_view = !app_context.render_context.depth_view;
		}
		
		///////////////////////////
		// Movement and rotation //
		///////////////////////////
		player_movement(&app_context.player, &app_context.world, delta_time);

		///////////////
		// Cast rays //
		///////////////
		cast_all_rays(&app_context.raycast_result, &app_context.world, &app_context.player);

		////////////
		// Render //
		////////////
		render_frame(&app_context.frame_render_context);
	}

	assets_unload(&app_context.assets);

	CloseWindow();
	return 0;
}
