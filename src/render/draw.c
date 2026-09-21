#include "draw.h"

/////////////
// Minimap //
/////////////

void draw_minimap(Texture texture) {
	DrawRectangle(
		0,
		0,
		RENDER_MINIMAP_WIDTH,
		RENDER_MINIMAP_HEIGHT,
		BLUE
	);

	DrawTextureRec(texture, (Rectangle){0, 0, (float)texture.width, -(float)texture.height}, (Vector2){0, 0}, WHITE);
}

void draw_minimap_to_texture(MinimapRenderContext *context) {
	const RaycastResult 	*raycast_result 	= context->raycast_result;
	const World 			*world 				= context->world;
	const Sprite 			*sprites			= context->assets->sprites;
	const EntityState		*entity_state 		= context->entity_state;

	BeginTextureMode(context->texture);
		ClearBackground(WHITE);

		draw_tiles_minimap(world, raycast_result);
		draw_player_minimap(context->player);
		draw_rays_minimap(raycast_result->rays, context->player);
		draw_entities_minimap(entity_state);
	EndTextureMode();
}

void draw_tiles_minimap(const World *world, const RaycastResult *raycast_result) {
	for (int i = 0; i < MAP_NUM_ROWS; ++i) {
		for (int j = 0; j < MAP_NUM_COLS; ++j) {
			if (world->tiles[i][j] == -1) {
				Rectangle line;
				line.x 		= MINIMAP_SCALE_FACTOR * j * TILE_SIZE;
				line.y 		= MINIMAP_SCALE_FACTOR * i * TILE_SIZE;
				line.width 	= MINIMAP_SCALE_FACTOR * TILE_SIZE; 
				line.height = MINIMAP_SCALE_FACTOR * TILE_SIZE;

				if (raycast_result->visible_tiles[i][j] == true) {
					DrawRectangleRec(
						line,
						ORANGE
					);
				}

				DrawRectangleLinesEx(
					line,
					0.5,
					BLACK
				);
			} else {
				Vector2 position;
				position.x = MINIMAP_SCALE_FACTOR * j * TILE_SIZE;
				position.y = MINIMAP_SCALE_FACTOR * i * TILE_SIZE;

				Vector2 size;
				size.x = MINIMAP_SCALE_FACTOR * TILE_SIZE;
				size.y = MINIMAP_SCALE_FACTOR * TILE_SIZE;

				DrawRectangleV(
					position,
					size,
					BLACK
				);
			}
		}
	}
}

void draw_player_minimap(Player *player) {
	Vector2 circle;
	circle.x = MINIMAP_SCALE_FACTOR * player->x;
	circle.y = MINIMAP_SCALE_FACTOR * player->y;

	DrawCircleV(
		circle,
		player->radius,
		RED
	);
}

void draw_entities_minimap(const EntityState *entity_state) {
	for (int i = 0; i < entity_state->count; i++) {
		const Entity *entities = entity_state->entities;

		Vector2 entity_pos;
		entity_pos.x = MINIMAP_SCALE_FACTOR * entities[i].x;
		entity_pos.y = MINIMAP_SCALE_FACTOR * entities[i].y;
		DrawCircleV(
			entity_pos,
			3,
			GREEN
		);
	}
}

void draw_rays_minimap(const RayData *rays, Player *player) {
	for (int i = 0; i < RAY_COUNT; ++i) {
		RayData ray = rays[i];

		Vector2 ray_start;
		ray_start.x = MINIMAP_SCALE_FACTOR * player->x;
		ray_start.y = MINIMAP_SCALE_FACTOR * player->y;

		Vector2 ray_endpoint;
		ray_endpoint.x = MINIMAP_SCALE_FACTOR * ray.x_endpoint;
		ray_endpoint.y = MINIMAP_SCALE_FACTOR * ray.y_endpoint;

		Color blu = BLUE;
		Color x;
		x.r = 0;
		x.g = 0;
		x.b = 255;
		x.a = 10;

		DrawLineV(ray_start, ray_endpoint, x);
	}
}

///////////
// Scene //
///////////

Color get_depth_gradient_with_distance(double distance) {
	float near_dist = 0.0f;
	float far_dist  = 800.0f;

	float t = (distance - near_dist) / (far_dist - near_dist);
	if (t < 0.0f) t = 0.0f;
	if (t > 1.0f) t = 1.0f;

	unsigned char color_gradient = (unsigned char)(255.0f * (1.0f - t));

	Color depth_color;
	depth_color.r = 255;
	depth_color.g = color_gradient;
	depth_color.b = color_gradient;
	depth_color.a = 255;

	return depth_color;
}

void draw_entities(RenderContext *context) {
	const RaycastResult *raycast_result 	= context->raycast_result;
	const Sprite *sprites 					= context->assets->sprites;
	const Player *player 					= context->player;
	const EntityState *entity_state			= context->entity_state;

	for (int i = 0; i < entity_state->count; ++i) {
		const EntityDraw *entity_draw = &entity_state->entity_draws[i];
		const int id = entity_draw->id;

		const Entity *entity = &entity_state->entities[id];

		if (!entity->visible || entity->depth <= 0.0) {
			continue;
		}

		double entity_height = (TILE_SIZE / entity->depth) * DIST_PROJ_PLANE;
		double entity_width  = entity_height;
		
		double entity_screen_center_x = (RENDER_TEXTURE_WIDTH / 2.0) + (entity->side_offset / entity->depth) * DIST_PROJ_PLANE;
		
		float entity_pos_x = entity_screen_center_x - (entity_width / 2.0);
		float entity_pos_y = (RENDER_TEXTURE_HEIGHT / 2.0) - (entity_height / 2.0);
		
		//entity_width because sprite size varies
		for (int x = 0; x < entity_width; ++x) {
			int screen_x = (int)(entity_pos_x + x);
			if (screen_x < 0 || screen_x >= RENDER_TEXTURE_WIDTH) {
				continue;
			}
		
			if (raycast_result->wall_distance_by_column[screen_x] < entity->depth) {
				continue;
			}
		
			int texture_x = (int)(((double)x / entity_width) * TILE_SIZE);
		
			Rectangle src;
			src.x = texture_x;
			src.y = 0;
			src.width = 1;
			src.height = TILE_SIZE;
		
			Rectangle dst;
			dst.x = screen_x;
			dst.y = entity_pos_y;
			dst.width = 1;
			dst.height = entity_height;
		
			if (context->depth_view) {
				Color entity_depth_color = get_depth_gradient_with_distance(entity->depth);
		
				DrawTexturePro(
					sprites[entity->sprite_index].depth_mask,
					src,
					dst,
					(Vector2){0, 0},
					0.0f,
					entity_depth_color
				);
			} else {
				DrawTexturePro(
					sprites[entity->sprite_index].texture,
					src,
					dst,
					(Vector2){0, 0},
					0.0f,
					WHITE
				);
			}
		}
	}
}

void draw_walls(RenderContext *context) {
	const RaycastResult *raycast_result = context->raycast_result;
	const Texture2D 	*textures 		= context->assets->textures;
	Player 				*player 		= context->player;

	for(int i = 0; i < RAY_COUNT; ++i) {
		RayData ray = raycast_result->rays[i];

		Texture2D actual_wall_texture = textures[ray.texture_id];

		double distance_to_projection_plane = DIST_PROJ_PLANE;
		double ray_distance 				= ray.distance * cos(ray.ray_angle - player->rotation_angle);
		double wall_strip_height 			= (TILE_SIZE / ray_distance) * distance_to_projection_plane;

		float x = i * WALL_STRIP_WIDTH;
		float y = (WINDOW_HEIGHT / 2.0) - (wall_strip_height / 2.0);
		
		if (context->depth_view) {
			Color depth_color = get_depth_gradient_with_distance(ray.distance);
			DrawRectangle(x, y, WALL_STRIP_WIDTH, wall_strip_height, depth_color); 
		} else {
			//source rectangle for getting the wall texture strip		
			Rectangle src;
			src.x = ray.texture_strip_index;
			src.y = 0;
			src.width = WALL_STRIP_WIDTH;
			src.height = TILE_SIZE;
			
			//destination rectangle for drawing the source strip 
			Rectangle dst;
			dst.x = i * WALL_STRIP_WIDTH;
			dst.y = (WINDOW_HEIGHT / 2.0) - (wall_strip_height / 2.0);
			dst.width = WALL_STRIP_WIDTH;
			dst.height = wall_strip_height;
			
			DrawTexturePro(actual_wall_texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
		}
	}
}

void draw_floor_and_ceiling_segment(void *job_context, int start, int end) {
	assert(job_context);

	FloorCeilingJobData *context = (FloorCeilingJobData *)job_context;

	Texture2D floor_ceiling_buffer_texture 	= context->assets->floor_ceiling_buffer_texture;
	Color *floor_ceiling_buffer_pixels 		= context->assets->floor_ceiling_buffer_pixels;
	const Color *floor_texture_pixels 		= context->assets->floor_pixels;
	const Color *ceiling_texture_pixels 	= context->assets->ceiling_pixels;
	int floor_texture_width 				= context->assets->floor_image.width;
	int floor_texture_height 				= context->assets->floor_image.height;

	int width 				= RENDER_TEXTURE_WIDTH;
	int height 				= RENDER_TEXTURE_HEIGHT;
	int horizon_y 			= height / 2;

	double forward_dir_x 	= context->player_forward_dir_x;
	double forward_dir_y 	= context->player_forward_dir_y;

	double proj_plane_dir_x = context->proj_plane_dir_x;
	double proj_plane_dir_y = context->proj_plane_dir_y;

	double player_tile_x 	= context->player_tile_x;
	double player_tile_y 	= context->player_tile_y;

	double ray_dir_x_left 	= context->ray_dir_x_left;
	double ray_dir_y_left 	= context->ray_dir_y_left;
	double ray_dir_x_right 	= context->ray_dir_x_right;
	double ray_dir_y_right 	= context->ray_dir_y_right;

	for (int rows_to_horizon = start; rows_to_horizon < end; ++rows_to_horizon) {
		int row = horizon_y + rows_to_horizon;

		//in tile-space
		double row_distance = (CAMERA_HEIGHT_IN_TILES * DIST_PROJ_PLANE) / rows_to_horizon;

		//compute world-space distance to next pixel
		double floor_step_x = row_distance * (ray_dir_x_right - ray_dir_x_left) / width;
		double floor_step_y = row_distance * (ray_dir_y_right - ray_dir_y_left) / width;

		//world-space floor point under the left-most screen pixel (in tile-space)
		double floor_x = player_tile_x + row_distance * ray_dir_x_left;
		double floor_y = player_tile_y + row_distance * ray_dir_y_left;

		int floor_row_start 	= row * width;
		int ceiling_row_start 	= (horizon_y - rows_to_horizon) * width;

		if (context->depth_view) {
			for (int x = 0; x < width; ++x) {
				double delta_x = floor_x - player_tile_x;
				double delta_y = floor_y - player_tile_y;
				double distance = sqrt(delta_x * delta_x + delta_y * delta_y);
				double world_distance = distance * TILE_SIZE;

				floor_ceiling_buffer_pixels[floor_row_start + x] 	= get_depth_gradient_with_distance(world_distance);
				floor_ceiling_buffer_pixels[ceiling_row_start + x] 	= get_depth_gradient_with_distance(world_distance);

				floor_x += floor_step_x;
				floor_y += floor_step_y;
			}
		} else {
			for (int x = 0; x < width; ++x) {
				int tile_x = (int)floor_x;
				int tile_y = (int)floor_y;

				//subtract int tile_x from double floor_x (this gives you a fraction inside the tile)
				double frac_x = floor_x - tile_x;
				double frac_y = floor_y - tile_y;

				//texture coordinates
				int texture_x = (int)(floor_texture_width  * frac_x);
				int texture_y = (int)(floor_texture_height * frac_y);

				//wrap
				if ((floor_texture_width & (floor_texture_width - 1)) == 0) {
					texture_x &= (floor_texture_width - 1);
				} else {
					texture_x %= floor_texture_width;
					if (texture_x < 0) texture_x += floor_texture_width;
				}

				if ((floor_texture_height & (floor_texture_height - 1)) == 0) {
					texture_y &= (floor_texture_height - 1);
				} else {
					texture_y %= floor_texture_height;
					if (texture_y < 0) texture_y += floor_texture_height;
				}

				//pixel color samples
				Color floor_color 	= floor_texture_pixels[texture_y * floor_texture_width + texture_x];
				Color ceiling_color = ceiling_texture_pixels[texture_y * floor_texture_width + texture_x];

				//darken
				floor_color.r = (unsigned char)(floor_color.r >> 1);
				floor_color.g = (unsigned char)(floor_color.g >> 1);
				floor_color.b = (unsigned char)(floor_color.b >> 1);

				ceiling_color.r = (unsigned char)(ceiling_color.r >> 1);
				ceiling_color.g = (unsigned char)(ceiling_color.g >> 1);
				ceiling_color.b = (unsigned char)(ceiling_color.b >> 1);

				floor_ceiling_buffer_pixels[floor_row_start + x] 	= floor_color;
				floor_ceiling_buffer_pixels[ceiling_row_start + x] 	= ceiling_color;

				//step by one screen pixel
				floor_x += floor_step_x;
				floor_y += floor_step_y;
			}
		}
	}
}

void draw_floor_and_ceiling(RenderContext *context) {
	Texture2D floor_ceiling_buffer_texture 	= context->assets->floor_ceiling_buffer_texture;
	Color *floor_ceiling_buffer_pixels 		= context->assets->floor_ceiling_buffer_pixels;
	const Color *floor_texture_pixels 		= context->assets->floor_pixels;
	const Color *ceiling_texture_pixels 	= context->assets->ceiling_pixels;
	int floor_texture_width 				= context->assets->floor_image.width;
	int floor_texture_height 				= context->assets->floor_image.height;
	Player *player							= context->player;

	int width 		= RENDER_TEXTURE_WIDTH;
	int height 		= RENDER_TEXTURE_HEIGHT;
	int horizon_y 	= height / 2;

	double forward_dir_x = cos(player->rotation_angle);
	double forward_dir_y = sin(player->rotation_angle);

	double proj_plane_scale = tan(FOV_ANGLE / 2.0);
	double proj_plane_dir_x = -sin(player->rotation_angle) * proj_plane_scale;
	double proj_plane_dir_y = cos(player->rotation_angle) * proj_plane_scale;

	double player_tile_x = player->x / TILE_SIZE;
	double player_tile_y = player->y / TILE_SIZE;

	double ray_dir_x_left 	= forward_dir_x - proj_plane_dir_x;
	double ray_dir_y_left 	= forward_dir_y - proj_plane_dir_y;
	double ray_dir_x_right 	= forward_dir_x + proj_plane_dir_x;
	double ray_dir_y_right 	= forward_dir_y + proj_plane_dir_y;

	for (int rows_to_horizon = 1; rows_to_horizon < (height / 2); ++rows_to_horizon) {

		int row = horizon_y + rows_to_horizon;

		//in tile-space
		double row_distance = (CAMERA_HEIGHT_IN_TILES * DIST_PROJ_PLANE) / rows_to_horizon;

		//compute world-space distance to next pixel
		double floor_step_x = row_distance * (ray_dir_x_right - ray_dir_x_left) / width;
		double floor_step_y = row_distance * (ray_dir_y_right - ray_dir_y_left) / width;

		//world-space floor point under the left-most screen pixel (in tile-space)
		double floor_x = player_tile_x + row_distance * ray_dir_x_left;
		double floor_y = player_tile_y + row_distance * ray_dir_y_left;

		int floor_row_start 	= row * width;
		int ceiling_row_start 	= (horizon_y - rows_to_horizon) * width;

		if (context->depth_view) {
			for (int x = 0; x < width; ++x) {
				double delta_x = floor_x - player_tile_x;
				double delta_y = floor_y - player_tile_y;
				double distance = sqrt(delta_x * delta_x + delta_y * delta_y);
				double world_distance = distance * TILE_SIZE;

				floor_ceiling_buffer_pixels[floor_row_start + x] 	= get_depth_gradient_with_distance(world_distance);
				floor_ceiling_buffer_pixels[ceiling_row_start + x] 	= get_depth_gradient_with_distance(world_distance);

				floor_x += floor_step_x;
				floor_y += floor_step_y;
			}
		} else {
			for (int x = 0; x < width; ++x) {
				int tile_x = (int)floor_x;
				int tile_y = (int)floor_y;

				//subtract int tile_x from double floor_x (this gives you a fraction inside the tile)
				double frac_x = floor_x - tile_x;
				double frac_y = floor_y - tile_y;

				//texture coordinates
				int texture_x = (int)(floor_texture_width  * frac_x);
				int texture_y = (int)(floor_texture_height * frac_y);

				//wrap
				if ((floor_texture_width & (floor_texture_width - 1)) == 0) {
					texture_x &= (floor_texture_width - 1);
				} else {
					texture_x %= floor_texture_width;
					if (texture_x < 0) texture_x += floor_texture_width;
				}

				if ((floor_texture_height & (floor_texture_height - 1)) == 0) {
					texture_y &= (floor_texture_height - 1);
				} else {
					texture_y %= floor_texture_height;
					if (texture_y < 0) texture_y += floor_texture_height;
				}

				//pixel color samples
				Color floor_color 	= floor_texture_pixels[texture_y * floor_texture_width + texture_x];
				Color ceiling_color = ceiling_texture_pixels[texture_y * floor_texture_width + texture_x];

				//darken
				floor_color.r = (unsigned char)(floor_color.r >> 1);
				floor_color.g = (unsigned char)(floor_color.g >> 1);
				floor_color.b = (unsigned char)(floor_color.b >> 1);

				ceiling_color.r = (unsigned char)(ceiling_color.r >> 1);
				ceiling_color.g = (unsigned char)(ceiling_color.g >> 1);
				ceiling_color.b = (unsigned char)(ceiling_color.b >> 1);

				floor_ceiling_buffer_pixels[floor_row_start + x] 	= floor_color;
				floor_ceiling_buffer_pixels[ceiling_row_start + x] 	= ceiling_color;

				//step by one screen pixel
				floor_x += floor_step_x;
				floor_y += floor_step_y;
			}
		}
	}

	UpdateTexture(floor_ceiling_buffer_texture, floor_ceiling_buffer_pixels);
	DrawTexture(floor_ceiling_buffer_texture, 0, 0, WHITE);
}