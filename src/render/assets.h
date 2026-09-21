#ifndef ASSETS_H
#define ASSETS_H

#include "raylib.h"
#include "base/common.h"

typedef struct Sprite Sprite;
struct Sprite {
	Texture2D texture;
	Texture2D depth_mask;
};

typedef struct Assets Assets;
struct Assets {
	Texture2D textures[NUM_TEXTURES];
	Sprite sprites[NUM_SPRITES];
	
	Image floor_image;
	Image ceiling_image;
	
	Color *floor_pixels;
	Color *ceiling_pixels;
	
	int floor_width;
	int floor_height;
	int ceiling_width;
	int ceiling_height;
	
	Image 		floor_ceiling_buffer_image;
	Color 	   *floor_ceiling_buffer_pixels;
	Texture2D 	floor_ceiling_buffer_texture;
};

bool assets_load			(Assets *assets);
void assets_unload			(Assets *assets);

#endif