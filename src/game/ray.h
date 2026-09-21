#ifndef RAY_C
#define RAY_C

#include "world.h"
#include "player.h"
#include "base/common.h"

typedef struct RayData RayData;
struct RayData {
	double 	ray_angle;
	double 	x_endpoint;
	double 	y_endpoint;
	double 	distance;
	bool	was_vertical_hit;
	int		texture_strip_index;
	int		texture_id;
};

typedef struct RaycastResult RaycastResult;
struct RaycastResult {
    RayData rays[RAY_COUNT];

    bool 	visible_tiles			[MAP_NUM_ROWS][MAP_NUM_COLS];
    double 	wall_distance_by_column	[RAY_COUNT];
};

double 	distance_between_points		(double x1, double y1, double x2, double y2);
void 	clear_visible_tiles			(RaycastResult *result);
void 	mark_visible_tiles			(RaycastResult *raycast_result, const World *world, Player *player, double ray_angle);
RayData cast_single_ray				(const World *world, Player *player, double ray_angle); 
void 	cast_all_rays				(RaycastResult *result, const World *world, Player *player);

#endif