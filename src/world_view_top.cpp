#include "world_view_top.h"
#include "draw.h"
#include "world.h"
#include "cube_top.h"
#include <memory.h>
#include <cmath>

static float g_cube_scale_width;
static float g_cube_scale_height;
static float g_cube_scale_depth;
static int g_world_height;
static int g_world_width;
static int g_world_depth;

Vec3 world_view_top_projected2world(const WorldViewTopCamera* camera, Vec2 projected_pos, float world_y);
Vec2 world_view_top_world2projected(const WorldViewTopCamera* camera, Vec3 world_pos);
Vec2 world_view_top_get_camera_offset(const WorldViewTopCamera* camera);
void world_view_top_draw_single(
    BlockInfo* block_info, const WorldViewTopCamera* camera,
    float x, float y, float z
);

// public start
void world_view_top_init() {
    g_world_height = world_get_height();
    g_world_width = world_get_width();
    g_world_depth = world_get_depth();
    g_cube_scale_width = cube_top_get_scale_width();
    g_cube_scale_height = cube_top_get_scale_height();
    g_cube_scale_depth = cube_top_get_scale_depth();
}

void world_view_top_init_camera(
    WorldViewTopCamera* camera,
    const Vec3 look_at,
    int screen_width, int screen_height,
    WorldViewTopDir dir
) {
    camera->look_at = look_at;
    camera->screen_width = screen_width;
    camera->screen_height = screen_height;
    camera->dir = dir;

    camera->camera_screen_pos = Coord2{ screen_width / 2, screen_height / 2 };
    camera->camera_offset = world_view_top_get_camera_offset(camera);
}

void world_view_top_set_focus(WorldViewTopCamera* camera, const Vec3 look_at) {
    camera->look_at = look_at;
    camera->camera_offset = world_view_top_get_camera_offset(camera);
}

void world_view_top_set_dir(WorldViewTopCamera* camera, WorldViewTopDir dir) {
    camera->dir = dir;
    camera->camera_offset = world_view_top_get_camera_offset(camera);
}

// Coordinates utils
Vec3 world_view_top_get_right(WorldViewTopDir dir) {
    switch (dir) {
    case WorldViewTopDir_Front: {
        return Vec3{ 1, 0, 0 };
    }
    case WorldViewTopDir_Right: {
        return Vec3{ 0, 0, 1 };
    }
    case WorldViewTopDir_Back: {
        return Vec3{ -1, 0, 0 };
    }
    case WorldViewTopDir_Left: {
        return Vec3{ 0, 0, -1 };
    }
    }
}

Vec3 world_view_top_get_forward(WorldViewTopDir dir) {
    switch (dir) {
    case WorldViewTopDir_Front: {
        return Vec3{ 0, 0, 1 };
    }
    case WorldViewTopDir_Right: {
        return Vec3{ -1, 0, 0 };
    }
    case WorldViewTopDir_Back: {
        return Vec3{ 0, 0, -1 };
    }
    case WorldViewTopDir_Left: {
        return Vec3{ 1, 0, 0 };
    }
    }
}

float world_view_top_get_rotation_radians(WorldViewTopDir dir) {
    switch (dir) {
    case WorldViewTopDir_Front: {
        return 0;
    }
    case WorldViewTopDir_Right: {
        return PI_HALF;
    }
    case WorldViewTopDir_Back: {
        return PI;
    }
    case WorldViewTopDir_Left: {
        return PI_HALF * 3.0f;
    }
    }
}

float world_view_top_get_z_value(WorldViewTopDir dir, float x, float z) {
    switch (dir) {
    case WorldViewTopDir_Front: {
        return g_world_depth - z;
    }
    case WorldViewTopDir_Right: {
        return x + 1;
    }
    case WorldViewTopDir_Back: {
        return z + 1;
    }
    case WorldViewTopDir_Left: {
        return g_world_width - x;
    }
    }
}

float world_view_top_get_z_value(WorldViewTopDir dir, Vec3 pos) {
    return world_view_top_get_z_value(dir, pos.x, pos.z);
}

Vec2 world_view_top_world2projected(const WorldViewTopCamera* camera, Vec3 world_pos) {
    switch (camera->dir) {
    case WorldViewTopDir_Front: {
        return Vec2{
            world_pos.x * g_cube_scale_width,
            -world_pos.z * g_cube_scale_depth - world_pos.y * g_cube_scale_height
        };
    }
    case WorldViewTopDir_Right: {
        return Vec2{
            world_pos.z * g_cube_scale_width,
            world_pos.x * g_cube_scale_depth - world_pos.y * g_cube_scale_height
        };
    }
    case WorldViewTopDir_Back: {
        return Vec2{
            -world_pos.x * g_cube_scale_width,
            world_pos.z * g_cube_scale_depth - world_pos.y * g_cube_scale_height
        };
    }
    case WorldViewTopDir_Left: {
        return Vec2{
            -world_pos.z * g_cube_scale_width,
            -world_pos.x * g_cube_scale_depth - world_pos.y * g_cube_scale_height
        };
    }
    }
}

Vec3 world_view_top_projected2world(const WorldViewTopCamera* camera, Vec2 projected_pos, float world_y)
{
    float x = 0.0f, z = 0.0f;
    switch (camera->dir) {
    case WorldViewTopDir_Front: {
        x = projected_pos.x / g_cube_scale_width;
        z = -(projected_pos.y + world_y * g_cube_scale_height) / g_cube_scale_depth;
        break;
    }
    case WorldViewTopDir_Right: {
        x = (projected_pos.y + world_y * g_cube_scale_height) / g_cube_scale_depth;
        z = projected_pos.x / g_cube_scale_width;
        break;
    }
    case WorldViewTopDir_Back: {
        x = -projected_pos.x / g_cube_scale_width;
        z = (projected_pos.y + world_y * g_cube_scale_height) / g_cube_scale_depth;
        break;
    }
    case WorldViewTopDir_Left: {
        x = -(projected_pos.y + world_y * g_cube_scale_height) / g_cube_scale_depth;
        z = -projected_pos.x / g_cube_scale_width;
        break;
    }
    }
    return Vec3{ x, world_y, z };
}

Vec3 world_view_top_screen2world(const WorldViewTopCamera* camera, Vec2 screen_pos, float world_y) {
    Vec2 projected = vec2_sub(screen_pos, camera->camera_offset);
    return world_view_top_projected2world(camera, projected, world_y);
}

Vec2 world_view_top_world2screen(const WorldViewTopCamera* camera, Vec3 world_pos) {
    return vec2_add(world_view_top_world2projected(camera, world_pos), camera->camera_offset);
}

Vec2 world_view_top_get_camera_offset(const WorldViewTopCamera* camera) {
    Vec2 screen_camera = world_view_top_world2projected(camera, camera->look_at);
    return Vec2{
        std::round(camera->camera_screen_pos.x - screen_camera.x),
        std::round(camera->camera_screen_pos.y - screen_camera.y + 0.5f) // magic
    };
}

Coord2 world_view_top_get_cube_offset(const WorldViewTopCamera* camera, float x, float y, float z) {
    Vec2 screen_origin = world_view_top_world2screen(camera, Vec3{ x, y, z });
    // TODO switch case
    switch (camera->dir) {
    case WorldViewTopDir_Front: {
        return Coord2{
            (int)(screen_origin.x),
            (int)(screen_origin.y - g_cube_scale_height * 3)
        };
    }
    case WorldViewTopDir_Right: {
        return Coord2{
            (int)(screen_origin.x),
            (int)(screen_origin.y - g_cube_scale_height)
        };
    }
    case WorldViewTopDir_Back: {
        return Coord2{
            (int)(screen_origin.x - g_cube_scale_width),
            (int)(screen_origin.y - g_cube_scale_height)
        };
    }
    case WorldViewTopDir_Left: {
        return Coord2{
            (int)(screen_origin.x - g_cube_scale_width),
            (int)(screen_origin.y - g_cube_scale_height * 3)
        };
    }
    }
}

// draw
void world_view_top_draw_single(
    BlockInfo* block_info,
    const WorldViewTopCamera* camera,
    float x, float y, float z
) {
    if (!world_get_visible_block_info_at(block_info, x, y, z)) {
        return;
    }
    bool top_visible = !world_get_visible_active_block_at(x, y + 1, z);
    Vec3 front_dir = world_view_top_get_forward(camera->dir);
    bool front_visible = !world_get_visible_active_block_at(x - front_dir.x, y, z - front_dir.z);
    if (!top_visible && !front_visible) {
        return;
    }
    int offset_y = 0;
    if (block_info->block->status == BlockStatus_Fall) {
		offset_y = block_info->block->offset_y;
    }
    Coord2 draw_offset = world_view_top_get_cube_offset(camera, x, y + offset_y, z);
 
    float depth_z = world_view_top_get_z_value(camera->dir, x, z);
    if (top_visible) {
        cube_top_draw_face_top(draw_offset.x, draw_offset.y, block_info, depth_z);
    }
    if (front_visible) {
        cube_top_draw_face_front(draw_offset.x, draw_offset.y, block_info, depth_z);
    }
}

void world_view_top_draw(const WorldViewTopCamera* camera) {
    buffer_z_clear();
    Vec3IntBoundary boundary = world_get_boundary();
    BlockInfo block_info;
    switch (camera->dir) {
    case WorldViewTopDir_Front:
    case WorldViewTopDir_Left: {
        for (int y = boundary.min_y; y < boundary.max_y; ++y) {
            for (int z = boundary.max_z - 1; z >= boundary.min_z; --z) {
                for (int x = boundary.max_x - 1; x >= boundary.min_x; --x) {
                    world_view_top_draw_single(&block_info, camera, x, y, z);
                }
            }
        }
        break;
    }
    case WorldViewTopDir_Right:
    case WorldViewTopDir_Back: {
        for (int y = boundary.min_y; y < boundary.max_y; ++y) {
            for (int z = boundary.min_z; z < boundary.max_z; ++z) {
                for (int x = boundary.min_x; x < boundary.max_x; ++x) {
                    world_view_top_draw_single(&block_info, camera, x, y, z);
                }
            }
        }
        break;
    }
    }
}

