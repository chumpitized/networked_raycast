#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>

#include "base/common.h"

#define WORLD_EMPTY_TILE -1

typedef struct World World;
struct World {
    int tiles[MAP_NUM_ROWS][MAP_NUM_COLS];
};

void 	world_init					(World *world);

bool 	world_contains_tile			(int tile_x, int tile_y);
bool 	world_contains_position		(double x, double y);

int 	world_get_tile				(const World *world, int tile_x, int tile_y);
void 	world_set_tile				(World *world, int tile_x, int tile_y, int tile_value);

bool 	world_has_wall				(const World *world, double x, double y);
int 	world_get_wall_texture_id	(const World *world, double x, double y);

#endif