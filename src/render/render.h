#ifndef RENDER_H
#define RENDER_H

#include "parallel/thread_pool.h"
#include "parallel/parallel_for.h"

#include "game/world.h"
#include "game/entity.h"

#include "draw.h"
#include "assets.h"

#include "raylib.h"

typedef struct FrameRenderContext FrameRenderContext;
struct FrameRenderContext {
	Player 					*player;
	RaycastResult 			*raycast_result;
	MinimapRenderContext 	*minimap_render_context;
	EntityState 			*entity_state;
	RenderContext 			*render_context;
	ThreadPool 				*thread_pool;
};

void render_frame(FrameRenderContext *context);

#endif