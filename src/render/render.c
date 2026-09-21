#include "render.h"

#include "raylib.h"
#include "draw.h"
#include "parallel/parallel_for.h"

void render_frame(FrameRenderContext *context) {
	Player *player 									= context->player;
	RaycastResult *raycast_result 					= context->raycast_result;
	MinimapRenderContext *minimap_render_context 	= context->minimap_render_context;	
	EntityState *entity_state 						= context->entity_state;
	RenderContext *render_context					= context->render_context;
	ThreadPool *thread_pool							= context->thread_pool;

	entity_draws_prepare(entity_state, raycast_result, player);

	draw_minimap_to_texture(minimap_render_context);

	BeginDrawing();
		ClearBackground(WHITE);
		
		//old flow using just main thread...
		//draw_floor_and_ceiling(&render_context);
		
		///////////////////////
		// Floor and Ceiling //
		///////////////////////
		double player_forward_dir_x = cos(player->rotation_angle);
		double player_forward_dir_y = sin(player->rotation_angle);
		
		double proj_plane_dir_x 	= -sin(player->rotation_angle) * PROJ_PLANE_SCALE;
		double proj_plane_dir_y 	= cos(player->rotation_angle) * PROJ_PLANE_SCALE;
		
		FloorCeilingJobData floor_ceiling_job_data = {
			.assets 				= render_context->assets,
		
			.player_tile_x 			= player->x / TILE_SIZE,
			.player_tile_y 			= player->y / TILE_SIZE,
				
			.player_forward_dir_x 	= player_forward_dir_x,
			.player_forward_dir_y 	= player_forward_dir_y,
		
			.proj_plane_dir_x 		= proj_plane_dir_x,
			.proj_plane_dir_y 		= proj_plane_dir_y,
		
			.ray_dir_x_left 		= player_forward_dir_x - proj_plane_dir_x,
			.ray_dir_y_left 		= player_forward_dir_y - proj_plane_dir_y,
			.ray_dir_x_right 		= player_forward_dir_x + proj_plane_dir_x,
			.ray_dir_y_right 		= player_forward_dir_y + proj_plane_dir_y,
		
			.depth_view 			= render_context->depth_view
		};
		
		parallel_for(
			thread_pool, 
			0, 
			RENDER_TEXTURE_HEIGHT / 2, 
			10, 
			draw_floor_and_ceiling_segment, 
			&floor_ceiling_job_data
		);
		
		UpdateTexture(
			render_context->assets->floor_ceiling_buffer_texture, 
			render_context->assets->floor_ceiling_buffer_pixels
		);
		DrawTexture(
			render_context->assets->floor_ceiling_buffer_texture, 
			0, 
			0, 
			WHITE
		);
		
		///////////
		// Walls //
		///////////
		draw_walls(render_context);
		
		//////////////
		// Entities //
		//////////////
		draw_entities(render_context);
		
		///////////////////
		// Minimap & FPS //
		///////////////////
		draw_minimap(minimap_render_context->texture.texture);
		DrawFPS(RENDER_TEXTURE_WIDTH - 75, 0);

	EndDrawing();
}
