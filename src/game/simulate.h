#ifndef SIMULATE_H
#define SIMULATE_H

#include "entity.h"
#include "npc_movement.h"

#include "parallel/parallel_queue.h"

#include "game/game_server.h"

void simulate(ParallelQueue *network_queue, EntityState *entity_state, double delta_time);

#endif
