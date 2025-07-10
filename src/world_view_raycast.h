#pragma once
#include "vec.h"
#include "cube.h"

struct Ray {
    Vec3 origin;
    Vec3 direction;
    // precompute
    Vec3 inv_direction;
    Vec3 step; // sign of direction
    Vec3 t_delta; // voxel size / direction
};

struct RayHitRecord {
    float t;
    Vec3 hit_pos;
    Vec3 block_pos;
    bool is_top;
    FaceInfo face_info;
	bool is_object;
    bool is_object_small;
	Pixel pixel;
};

// camera start
struct WorldViewRayCamera {
    // config general
    Vec3 look_at;
    int screen_width;
    int screen_height;
    // config
    float radians;
    Vec3 direction;
    // calculated
    // for raycast
    Vec3 pos_top_left;
    Vec3 u;
    Vec3 v;
    // for shadow
    float sin_radians;
    float cos_radians;
    Vec3 grid_forward; // screen forward in world
    Vec3 grid_right; // screen right in world
};

void world_view_ray_init_camera(WorldViewRayCamera* camera,
    const Vec3 look_at, float screen_width, float screen_height, float radians);

void world_view_ray_draw(WorldViewRayCamera* camera);

void world_view_ray_set_render_object(bool render_object);