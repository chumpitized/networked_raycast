#include "entity.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "base/sort.h"

void init_entity_state(EntityState *state, int count) {
	srand((unsigned int)time(NULL));

	state->entities 	= malloc(sizeof(Entity) * count);
	state->entity_draws = malloc(sizeof(EntityDraw) * count);
	state->count 		= count;

	for (int i = 0; i < count; ++i) {
		Entity new_entity = {0};
		new_entity.sprite_index = rand() % NUM_SPRITES;

		double spawn_x = (rand() % (TILE_SIZE * MAP_NUM_COLS));
		double spawn_y = (rand() % (TILE_SIZE * MAP_NUM_ROWS));

		if (spawn_x < TILE_SIZE * 2) spawn_x += TILE_SIZE * 2;
		if (spawn_y < TILE_SIZE * 2) spawn_y += TILE_SIZE * 2;

		if (spawn_x > TILE_SIZE * (MAP_NUM_ROWS - 2)) spawn_x -= TILE_SIZE * 2;
		if (spawn_y > TILE_SIZE * (MAP_NUM_ROWS - 2)) spawn_y -= TILE_SIZE * 2;

		new_entity.x 		= spawn_x;
		new_entity.y 		= spawn_y;
		new_entity.forward	= (rand() & 1);

		state->entities[i] 			= new_entity;
		state->entity_draws[i].id	= i;
	}
}

static int cmp_entity_draw(const void *a, const void *b) {
	double ai = ((const EntityDraw *)a)->depth;
	double bi = ((const EntityDraw *)b)->depth;

	return (ai < bi) - (ai > bi);
}

void entity_draws_sort(EntityState *state) {
	qsort_generic(state->entity_draws, sizeof(EntityDraw), 0, state->count - 1, cmp_entity_draw);
}

void entity_draws_prepare(EntityState *entity_state, const RaycastResult *raycast_result, const Player *player) {
	double forward_x = cos(player->rotation_angle);
	double forward_y = sin(player->rotation_angle);
	double right_x   = -sin(player->rotation_angle);
	double right_y   = cos(player->rotation_angle);

	for (int i = 0; i < entity_state->count; ++i) {
		EntityDraw 	*entity_draw 	= &entity_state->entity_draws[i];
		//this is really the index into the entities array... not an id per se...
		int 		id 				= entity_draw->id;

		Entity 		*entity 		= &entity_state->entities[id];

		int tile_x 	= (int)entity->x / TILE_SIZE;
		int tile_y 	= (int)entity->y / TILE_SIZE;

		if (raycast_result->visible_tiles[tile_y][tile_x]) {
			entity->visible = true;

			double delta_x = entity->x - player->x;
			double delta_y = entity->y - player->y;

			float depth 		= delta_x * forward_x + delta_y * forward_y;
			float side_offset	= delta_x * right_x + delta_y * right_y;

			entity->depth 		= depth;
			entity->side_offset = side_offset;

			entity_draw->depth 	= depth;
		} else {
			entity->visible = false;
		}
	}

	entity_draws_sort(entity_state);
}