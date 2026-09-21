#include "simulate.h"

void simulate(ParallelQueue *network_queue, EntityState *entity_state, double delta_time) {
	//we can use the original parallel queue for now, but we should switch to cached...

	PlayerMove player_move;
	bool consumed = consume_parallel_queue(network_queue, &player_move);

	while (consumed) {

		move_random_npc(entity_state, player_move.player_id, delta_time);

		consumed = consume_parallel_queue(network_queue, &player_move);
	}

}