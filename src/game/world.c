#include "world.h"

int default_tilemap[MAP_NUM_ROWS][MAP_NUM_COLS] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1},
	{1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1},
	{1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1},
	{1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1},
	{4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1},
	{2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1},
	{1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1},
	{4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1},
	{9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void world_init(World *world) {
	memcpy(world->tiles, default_tilemap, sizeof(default_tilemap));
}

bool world_contains_tile(int tile_x, int tile_y) {
	return (tile_x >= 0 && tile_x < MAP_NUM_COLS) && (tile_y >= 0 && tile_y < MAP_NUM_ROWS);
}

bool world_contains_position(double x, double y) {
	return (x >= 0 && x < MAP_NUM_COLS * TILE_SIZE) && (y >= 0 && y < MAP_NUM_ROWS * TILE_SIZE);
}
	
int world_get_tile(const World *world, int tile_x, int tile_y) {
	if (!world_contains_tile(tile_x, tile_y)) return -1;

	return world->tiles[tile_y][tile_x];
}

void world_set_tile(World *world, int tile_x, int tile_y, int tile_value) {
	if (!world_contains_tile(tile_x, tile_y)) return;

	world->tiles[tile_y][tile_x] = tile_value;
}

bool world_has_wall(const World *world, double x, double y) {
	if (!world_contains_position(x, y)) return false;

	int tile_x = (int)floor(x / TILE_SIZE);
	int tile_y = (int)floor(y / TILE_SIZE);

	return world->tiles[tile_y][tile_x] != -1;
}

int world_get_wall_texture_id(const World *world, double x, double y) {
	if (!world_contains_position(x, y)) return false;

	int tile_x = (int)floor(x / TILE_SIZE);
	int tile_y = (int)floor(y / TILE_SIZE);

	return world->tiles[tile_y][tile_x];
}