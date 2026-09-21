#ifndef PLAYER_H
#define PLAYER_H

#include "base/common.h"
#include "world.h"

typedef struct Player Player;
struct Player {
	double 	x;
	double 	y;
	int 	radius;
	int 	turn_direction;
	int		move_direction;
	int		strafe_direction;
	double 	rotation_angle;
	double 	move_speed;
	double 	rotation_speed;
};

void player_init_default	(Player *player);
void player_input			(Player *player);
void player_movement		(Player *player, const World *world, double delta_time);

#endif