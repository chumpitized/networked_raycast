#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <stdint.h>

#include "raylib.h"

#define TILE_SIZE 				64
#define MAP_NUM_ROWS 			11
#define MAP_NUM_COLS 			15

#define WINDOW_WIDTH 			(MAP_NUM_COLS * TILE_SIZE)
#define WINDOW_HEIGHT			(MAP_NUM_ROWS * TILE_SIZE)

#define FOV_ANGLE 				(60 * (PI / 180))

#define DIST_PROJ_PLANE 		((WINDOW_WIDTH / 2.0) / tan(FOV_ANGLE / 2.0))
#define PROJ_PLANE_SCALE		(tan(FOV_ANGLE / 2.0))

#define WALL_STRIP_WIDTH 		1
#define RAY_COUNT 				(RENDER_TEXTURE_WIDTH / WALL_STRIP_WIDTH)

#define NUM_TEXTURES 			12
#define NUM_SPRITES 			2
#define NUM_ENTITIES			2

#define FLOOR_TEXTURE_INDEX 	11
#define CEILING_TEXTURE_INDEX 	6

#define SPRITE_ORANGE_CAT		0
#define SPRITE_GRAY_CAT			1

#define RENDER_TEXTURE_WIDTH 	WINDOW_WIDTH
#define RENDER_TEXTURE_HEIGHT 	WINDOW_HEIGHT

#define RENDER_MINIMAP_WIDTH 	(RENDER_TEXTURE_WIDTH * MINIMAP_SCALE_FACTOR)
#define RENDER_MINIMAP_HEIGHT 	(RENDER_TEXTURE_HEIGHT * MINIMAP_SCALE_FACTOR)
#define MINIMAP_SCALE_FACTOR 	0.2f

#define TEXTURE_WIDTH 			TILE_SIZE
#define TEXTURE_HEIGHT 			TILE_SIZE

#define CAMERA_HEIGHT 			(TILE_SIZE / 2)
#define CAMERA_HEIGHT_IN_TILES 	(CAMERA_HEIGHT / (double)TILE_SIZE)

#endif