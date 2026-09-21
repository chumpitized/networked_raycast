#ifndef DRAW_H
#define DRAW_H

#include "base/common.h"

#include "game/ray.h"
#include "game/entity.h"

#include "assets.h"

typedef struct FloorCeilingJobData FloorCeilingJobData;
struct FloorCeilingJobData {
	const Assets *assets;

	double player_tile_x;
	double player_tile_y;

	double player_forward_dir_x;
	double player_forward_dir_y;

	double proj_plane_dir_x;
	double proj_plane_dir_y;

	double ray_dir_x_left; 
	double ray_dir_y_left; 
	double ray_dir_x_right;
	double ray_dir_y_right;

	bool depth_view;
};


typedef struct MinimapRenderContext MinimapRenderContext;
struct MinimapRenderContext {
	RenderTexture2D 		texture;

	const RaycastResult    *raycast_result;
	const Assets 		   *assets;
	const World 		   *world;
	const EntityState	   *entity_state;

	Player 				   *player;
};

typedef struct RenderContext RenderContext;
struct RenderContext {
	RenderTexture2D 		texture;

	const RaycastResult    *raycast_result;
	const Assets 		   *assets;
	const World 		   *world;
	const EntityState	   *entity_state;

	Player 				   *player;

	bool					depth_view;
};

Color 	get_depth_gradient_with_distance	(double distance);

void 	draw_minimap						(Texture texture);
void 	draw_minimap_to_texture				(MinimapRenderContext *context);
void 	draw_tiles_minimap					(const World *world, const RaycastResult *raycast_result);
void 	draw_player_minimap					(Player *player);
void 	draw_entities_minimap				(const EntityState *entity_state);
void 	draw_rays_minimap					(const RayData *rays, Player *player);

void 	draw_scene 							(RenderContext *context);
void 	draw_entities						(RenderContext *context);
void 	draw_walls							(RenderContext *context);
void 	draw_floor_and_ceiling				(RenderContext *context);
void 	draw_floor_and_ceiling_segment		(void *render_context, int start, int end);

#endif