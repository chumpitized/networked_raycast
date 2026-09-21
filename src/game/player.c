#include "player.h"

#include "render/draw.h"

void player_init_default(Player *player) {
	player->x 					= RENDER_TEXTURE_WIDTH / 2;
	player->y 					= RENDER_TEXTURE_HEIGHT / 2;
	player->radius 				= 3;
	player->turn_direction 		= 0;
	player->move_direction 		= 0;
	player->strafe_direction 	= 0;
	player->rotation_angle 		= PI * 1.5;
	player->move_speed 			= 240.0;
	player->rotation_speed 		= 2 * (PI / 180);
}

void player_input(Player *player) {
	player->move_direction 		= 0;
	player->strafe_direction 	= 0;
	player->turn_direction 		= 0;

	if (IsKeyDown(KEY_W)) {
		player->move_direction += 1;
	}

	if (IsKeyDown(KEY_S)) {
		player->move_direction -= 1;
	}

	if (IsKeyDown(KEY_D)) {
		player->strafe_direction += 1;
	}

	if (IsKeyDown(KEY_A)) {
		player->strafe_direction -= 1;
	}

	Vector2 mouse_delta 		= GetMouseDelta();
	double mouse_sensitivity 	= 0.003;
	player->rotation_angle 	   += mouse_delta.x * mouse_sensitivity;
}

void player_movement(Player *player, const World *world, double delta_time) {
    player->rotation_angle += player->turn_direction * player->rotation_speed;

    player->rotation_angle = fmod(player->rotation_angle, 2 * PI);
    if (player->rotation_angle < 0) {
        player->rotation_angle += 2 * PI;
    }

    double forward_x = cos(player->rotation_angle);
    double forward_y = sin(player->rotation_angle);

    double right_x = -sin(player->rotation_angle);
    double right_y =  cos(player->rotation_angle);

    double move_x = forward_x * player->move_direction + right_x * player->strafe_direction;
    double move_y = forward_y * player->move_direction + right_y * player->strafe_direction;

    double move_length = sqrt(move_x * move_x + move_y * move_y);

    if (move_length > 0.0) {
        move_x /= move_length;
        move_y /= move_length;
    }

	double new_x = player->x + move_x * (player->move_speed * delta_time);
	double new_y = player->y + move_y * (player->move_speed * delta_time);

	int screen_space_x = (int)(new_x + (new_x > player->x ? player->radius : -player->radius)) / TILE_SIZE;
	int screen_space_y = (int)player->y / TILE_SIZE;

	if (world->tiles[screen_space_y][screen_space_x] == -1) {
	    player->x = new_x;
	}

    screen_space_x = (int)player->x / TILE_SIZE;
	screen_space_y = (int)(new_y + (new_y > player->y ? player->radius : -player->radius)) / TILE_SIZE;

	if (world->tiles[screen_space_y][screen_space_x] == -1) {
	    player->y = new_y;
	}
}