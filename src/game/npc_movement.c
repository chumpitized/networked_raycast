#include "npc_movement.h"

//id is entity index
void move_random_npc(EntityState *entity_state, int id, double delta_time) {
	Entity *entity = &entity_state->entities[id];
	/*double x_move = entity->x_move;
	double y_move = entity->y_move;*/

	double move_speed = 480.0;
	/*double move_speed = 3600.0;*/

	if (entity->x >= TILE_SIZE * (MAP_NUM_COLS - 2)) {
		entity->forward = false;
	} else if (entity->x < TILE_SIZE * 2) {
		entity->forward = true;
	}

	double dx = entity->forward ? 1.0 : -1.0;
/*	double dy = entity->forward ? 1.0 : -1.0;

	double length = sqrt(dx * dx + dy * dy);

	if (length > 0.0) {
		dx /= length;
		dy /= length;
	}*/

	entity->x += (dx * move_speed) * delta_time;
	/*entity->y += dy * move_speed * delta_time;*/
}