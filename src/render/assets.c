#include "assets.h"

#include <assert.h>
#include <string.h>

static Texture2D make_sprite_depth_mask_from_image(Image sprite_image) {
    Image mask_image = GenImageColor(
        sprite_image.width,
        sprite_image.height,
        BLANK
    );

    ImageFormat(&mask_image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    Color *sprite_pixels = (Color *)sprite_image.data;
    Color *mask_pixels   = (Color *)mask_image.data;

    assert(sprite_pixels != NULL);
    assert(mask_pixels != NULL);

    for (int y = 0; y < sprite_image.height; ++y) {
        for (int x = 0; x < sprite_image.width; ++x) {
            int index = x + y * sprite_image.width;

            if (sprite_pixels[index].a > 0) {
                mask_pixels[index] = WHITE;
            }
        }
    }

    Texture2D mask_texture = LoadTextureFromImage(mask_image);

    UnloadImage(mask_image);

    return mask_texture;
}

bool assets_load(Assets *assets) {
    memset(assets, 0, sizeof(*assets));

    const char *texture_paths[NUM_TEXTURES] = {
		"textures/BLOCK_1A.png",
        "textures/BLOCK_1B.png",
		"textures/BLOCK_1C.png",
		"textures/BLOCK_1E.png",
		"textures/BLOCK_1F.png",
        "textures/BLOCK_3B.png",
        "textures/WOOD_1E.png",
		"textures/WOOD_2A.png",
        "textures/TILE_3D.png",
		"textures/SQDOOR_7E.png",
		"textures/COBBLES_4A.png",
		"textures/COBBLES_1A.png"
    };

	const char *sprite_paths[NUM_SPRITES] = {
		"textures/cat_orange.png",
		"textures/cat_gray.png"
	};

    for (int i = 0; i < NUM_TEXTURES; ++i) {
        Image img = LoadImage(texture_paths[i]);

        if (img.data == NULL) {
            return false;
        }

        if (i == FLOOR_TEXTURE_INDEX) {
            ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

            assets->floor_image 	= img;
            assets->floor_pixels 	= (Color *)assets->floor_image.data;
            assets->floor_width 	= assets->floor_image.width;
            assets->floor_height 	= assets->floor_image.height;
            assets->textures[i] 	= LoadTextureFromImage(assets->floor_image);
        } else if (i == CEILING_TEXTURE_INDEX) {
            ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

            assets->ceiling_image 	= img;
            assets->ceiling_pixels 	= (Color *)assets->ceiling_image.data;
            assets->ceiling_width 	= assets->ceiling_image.width;
            assets->ceiling_height 	= assets->ceiling_image.height;
            assets->textures[i] 	= LoadTextureFromImage(assets->ceiling_image);
        } else {
            assets->textures[i] = LoadTextureFromImage(img);
            UnloadImage(img);
        }
    }

	for (int i = 0; i < NUM_SPRITES; ++i) {
		Image img = LoadImage(sprite_paths[i]);

        if (img.data == NULL) {
            return false;
        }

		ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

		assets->sprites[i].texture 		= LoadTextureFromImage(img);
		assets->sprites[i].depth_mask 	= make_sprite_depth_mask_from_image(img);
		
		UnloadImage(img);
	}

    assets->floor_ceiling_buffer_image = GenImageColor(RENDER_TEXTURE_WIDTH, RENDER_TEXTURE_HEIGHT, BLANK);

    ImageFormat(&assets->floor_ceiling_buffer_image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    assets->floor_ceiling_buffer_texture 	= LoadTextureFromImage(assets->floor_ceiling_buffer_image);
    assets->floor_ceiling_buffer_pixels 	= (Color *)assets->floor_ceiling_buffer_image.data;

    assert(assets->floor_pixels != NULL);
    assert(assets->ceiling_pixels != NULL);
    assert(assets->floor_ceiling_buffer_pixels != NULL);

    assert(assets->floor_width > 0);
    assert(assets->floor_height > 0);
    assert(assets->ceiling_width > 0);
    assert(assets->ceiling_height > 0);

    return true;
}

void assets_unload(Assets *assets) {
	UnloadImage(assets->floor_ceiling_buffer_image);
	UnloadImage(assets->floor_image);
	UnloadImage(assets->ceiling_image);
	
	UnloadTexture(assets->floor_ceiling_buffer_texture);

	for (int i = 0; i < NUM_TEXTURES; ++i) {
		UnloadTexture(assets->textures[i]);
	}

	for (int i = 0; i < NUM_SPRITES; ++i) {
		UnloadTexture(assets->sprites[i].texture);
		UnloadTexture(assets->sprites[i].depth_mask);
	}
}