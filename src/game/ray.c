#include "ray.h"

double distance_between_points(double x1, double y1, double x2, double y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void mark_visible_tiles(RaycastResult *raycast_result, const World *world, Player *player, double ray_angle) {
	double ray_dir_x = cos(ray_angle);
	double ray_dir_y = sin(ray_angle);

	int tile_x = (int)floor(player->x / TILE_SIZE);
	int tile_y = (int)floor(player->y / TILE_SIZE);

	if (!world_contains_tile(tile_x, tile_y)) {
		return;
	}

	int step_x = (ray_dir_x > 0) ? 1 : -1;
	int step_y = (ray_dir_y > 0) ? 1 : -1;

	double tile_delta_x = (ray_dir_x == 0) ? DBL_MAX : fabs(TILE_SIZE / ray_dir_x);
	double tile_delta_y = (ray_dir_y == 0) ? DBL_MAX : fabs(TILE_SIZE / ray_dir_y);

	double next_vertical_grid_x 	= (ray_dir_x > 0) ? (tile_x + 1) * TILE_SIZE : tile_x * TILE_SIZE;
	double next_horizontal_grid_y 	= (ray_dir_y > 0) ? (tile_y + 1) * TILE_SIZE : tile_y * TILE_SIZE;

	double tile_first_delta_x = (ray_dir_x == 0) ? DBL_MAX : fabs((next_vertical_grid_x - player->x) / ray_dir_x);
	double tile_first_delta_y = (ray_dir_y == 0) ? DBL_MAX : fabs((next_horizontal_grid_y - player->y) / ray_dir_y);

	while (world_contains_tile(tile_x, tile_y)) {
		raycast_result->visible_tiles[tile_y][tile_x] = true;

		if (world->tiles[tile_y][tile_x] >= 0) {
			break;
		}

		if (tile_first_delta_x < tile_first_delta_y) {
			tile_x 				+= step_x;
			tile_first_delta_x 	+= tile_delta_x;
		} else {
			tile_y 				+= step_y;
			tile_first_delta_y 	+= tile_delta_y;
		}
	}
}

void clear_visible_tiles(RaycastResult *result) {
	for (int i = 0; i < MAP_NUM_ROWS; ++i) {
		for (int j = 0; j < MAP_NUM_COLS; ++j) {
			result->visible_tiles[i][j] = false;
		}
	}
}

RayData cast_single_ray(const World *world, Player *player, double ray_angle) {
	RayData ray = {0};
	ray.ray_angle = ray_angle;
	
	bool is_ray_down	= ray.ray_angle > 0 && ray.ray_angle < PI;
	bool is_ray_up 		= !is_ray_down;
	bool is_ray_right 	= ray.ray_angle < (PI * 0.5) || ray.ray_angle > (PI * 1.5);
	bool is_ray_left	= !is_ray_right;
	
	///////////////////////////
	// Horizontal collisions //
	///////////////////////////
	
	bool 	horizontal_collision 	= false;
	double 	x_horizontal_collision 	= 0; 
	double 	y_horizontal_collision 	= 0;
	
	double 	x_step, y_step;
	double 	x_intercept, y_intercept;
	int 	horizontal_texture_id;
	
	y_intercept 	= floor(player->y / TILE_SIZE) * TILE_SIZE;
	y_intercept    += is_ray_down ? TILE_SIZE : 0;
	x_intercept 	= player->x + (y_intercept - player->y) / tan(ray.ray_angle);
	
	y_step 	= is_ray_up ? -TILE_SIZE : TILE_SIZE;
	x_step 	= TILE_SIZE / tan(ray.ray_angle);
	x_step *= (is_ray_left && x_step > 0) ? -1 : 1;
	x_step *= (is_ray_right && x_step < 0) ? -1 : 1;
		
	double y_next_horizontal = y_intercept;
	double x_next_horizontal = x_intercept;
	
	while (x_next_horizontal >= 0 && x_next_horizontal < RENDER_TEXTURE_WIDTH && y_next_horizontal > 0 && y_next_horizontal <= RENDER_TEXTURE_HEIGHT) {
		double y_tile = y_next_horizontal - (is_ray_up ? 1 : 0);
		double x_tile = x_next_horizontal;
	
		if (world_has_wall(world, x_tile, y_tile)) {					
			horizontal_collision = true;
			horizontal_texture_id = world_get_wall_texture_id(world, x_tile, y_tile);
			y_horizontal_collision = y_next_horizontal;
			x_horizontal_collision = x_next_horizontal;
			break;
		} else {
			y_next_horizontal += y_step;
			x_next_horizontal += x_step;
		}
	}
	
	/////////////////////////
	// Vertical collisions //
	/////////////////////////
	
	bool 	vertical_collision 		= false;
	double 	x_vertical_collision 	= 0;
	double 	y_vertical_collision 	= 0;
	int 	vertical_texture_id;
	
	x_intercept 	= floor(player->x / TILE_SIZE) * TILE_SIZE;
	x_intercept    += is_ray_right ? TILE_SIZE : 0;
	y_intercept 	= player->y + (x_intercept - player->x) * tan(ray.ray_angle);
	
	x_step 	= is_ray_right ? TILE_SIZE : -TILE_SIZE;
	y_step 	= TILE_SIZE * tan(ray.ray_angle);
	y_step *= (is_ray_up && y_step > 0) ? -1 : 1;
	y_step *= (!is_ray_up && y_step < 0) ? -1 : 1;
	
	double x_next_vertical = x_intercept;
	double y_next_vertical = y_intercept;
	
	while (x_next_vertical >= 0 && x_next_vertical < RENDER_TEXTURE_WIDTH && y_next_vertical > 0 && y_next_vertical <= RENDER_TEXTURE_HEIGHT) {
		double x_tile = x_next_vertical - (is_ray_left ? 1 : 0);
		double y_tile = y_next_vertical;
			
		if (world_has_wall(world, x_tile, y_tile)) {
			vertical_collision = true;
			vertical_texture_id = world_get_wall_texture_id(world, x_tile, y_tile);
			y_vertical_collision = y_next_vertical;
			x_vertical_collision = x_next_vertical;
			break;
		} else {
			y_next_vertical += y_step;
			x_next_vertical += x_step;
		}
	}
	
	double horizontal_distance 	= horizontal_collision ? distance_between_points(player->x, player->y, x_horizontal_collision, y_horizontal_collision) : DBL_MAX;
	double vertical_distance	= vertical_collision ? distance_between_points(player->x, player->y, x_vertical_collision, y_vertical_collision) : DBL_MAX;
	
	ray.x_endpoint 			= (horizontal_distance < vertical_distance) ? x_horizontal_collision : x_vertical_collision;
	ray.y_endpoint 			= (horizontal_distance < vertical_distance) ? y_horizontal_collision : y_vertical_collision;
	ray.distance 			= (horizontal_distance < vertical_distance) ? horizontal_distance : vertical_distance;
	ray.was_vertical_hit 	= (vertical_distance < horizontal_distance);
	ray.texture_id 			= (vertical_distance < horizontal_distance) ? vertical_texture_id : horizontal_texture_id;
	
	double texture_pixel_index;
			
	if (ray.was_vertical_hit) {
		texture_pixel_index = is_ray_right ? fmod(y_vertical_collision, TILE_SIZE) : TILE_SIZE - fmod(y_vertical_collision, TILE_SIZE);
	} else {
		texture_pixel_index = is_ray_up ? fmod(x_horizontal_collision, TILE_SIZE) : TILE_SIZE - fmod(x_horizontal_collision, TILE_SIZE);
	}
	
	ray.texture_strip_index = (int)floor(texture_pixel_index);

	return ray;
}

void cast_all_rays(RaycastResult *raycast_result, const World *world, Player *player) {
	clear_visible_tiles(raycast_result);

	for (int i = 0; i < RAY_COUNT; ++i) {
		double screen_x = (i + 0.5) * WALL_STRIP_WIDTH - (WINDOW_WIDTH / 2.0);

		double ray_offset = atan(screen_x / DIST_PROJ_PLANE);

		double normalized_ray_angle = fmod(player->rotation_angle + ray_offset + 2.0 * PI, 2.0 * PI);
	
		if (normalized_ray_angle < 0) {
			normalized_ray_angle += 2 * PI;
		}

		mark_visible_tiles(raycast_result, world, player, normalized_ray_angle);

		raycast_result->rays[i] = cast_single_ray(world, player, normalized_ray_angle);

		raycast_result->wall_distance_by_column[i] = raycast_result->rays[i].distance;
	}
}