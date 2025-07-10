#pragma once
#include "vec.h"

enum WorldViewTopDir {
    WorldViewTopDir_Front,
    WorldViewTopDir_Right,
    WorldViewTopDir_Back,
    WorldViewTopDir_Left,
};

struct WorldViewTopCamera {
    Vec3 look_at;
    int screen_width;
    int screen_height;

    WorldViewTopDir dir;
    // computed
	Coord2 camera_screen_pos;
    Vec2 camera_offset;
};

// camear settings
void world_view_top_init();
void world_view_top_init_camera(
	WorldViewTopCamera* camera,
	const Vec3 look_at,
	int screen_width, int screen_height,
	WorldViewTopDir dir
);
void world_view_top_set_focus(WorldViewTopCamera* camera, const Vec3 look_at);
void world_view_top_set_dir(WorldViewTopCamera* camera, WorldViewTopDir dir);
// direction
Vec3 world_view_top_get_right(WorldViewTopDir dir);
Vec3 world_view_top_get_forward(WorldViewTopDir dir);
float world_view_top_get_rotation_radians(WorldViewTopDir dir);
float world_view_top_get_z_value(WorldViewTopDir dir, float x, float z);
// draw world
void world_view_top_draw(const WorldViewTopCamera* camera);
// coordinates
Vec3 world_view_top_screen2world(const WorldViewTopCamera* camera, Vec2 screen_pos, float world_y);
Vec2 world_view_top_world2screen(const WorldViewTopCamera* camera, Vec3 world_pos);
Coord2 world_view_top_get_cube_offset(const WorldViewTopCamera* camera, float x, float y, float z);