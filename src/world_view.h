#pragma once
#include "world_view_top.h"
#include "world_view_raycast.h"
enum WorldViewType {
    WorldViewType_Top, // top-down
    WorldViewType_Ray, // ray casting
};

void world_view_init();
void world_view_set_focus(Vec3 look_at);
WorldViewTopCamera* world_view_get_camera_top();
Vec3 world_view_get_forward();
Vec3 world_view_get_right();
WorldViewRayCamera* world_view_get_camera_ray();

// rotation
bool world_view_if_rotating();
void world_view_start_rotation(int direction);
void world_view_update();
// draw
void world_view_draw();