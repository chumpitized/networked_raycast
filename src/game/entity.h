#ifndef ENTITY_H
#define ENTITY_H

#include "base/common.h"
#include "player.h"
#include "ray.h"

#define MAX_ENTITIES 64

typedef struct Entity Entity;
struct Entity {
	double 	x;
	double 	y;

	bool 	forward;

	int 	sprite_index;

	bool 	visible;
	double 	depth;
	double 	side_offset;
};

typedef struct EntityDraw EntityDraw;
struct EntityDraw {
	//we use the depth here to SORT these `EntityDraw`s... THESE ARE WHAT GETS SORTED,
	//not the `Entity`s! This way we keep the heavier objects, Entity, in their own static
	//array and simply sort the EntityDraws by depth, with hold handles into static storage...
	double depth;
	//this id is a handle into static entity storage...
	int 	id;
	//We could get away with 16-bit IDs,
	//but that would add 2 bytes of padding,
	//making it effectively pointless.
};

typedef struct EntityState EntityState;
struct EntityState {
    Entity 	   *entities;
	EntityDraw *entity_draws;
    int 		count;
};

void init_entity_state		(EntityState *state, int count);
void entity_draws_sort		(EntityState *state);
void entity_draws_prepare	(EntityState *entity_state, const RaycastResult *raycast_result, const Player *player);

#endif