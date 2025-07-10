#pragma once
#include "world_view.h"
#include "draw.h" // buffer
#include "timer.h"
#include <cmath>
#include <memory.h> // memset

struct WorldViewConfig {
    // general camera
    Vec3 loot_at;
    int screen_width;
    int screen_height;
    // view status
    WorldViewType view_type;
    WorldViewTopDir view_top_dir;
    float view_ray_rotation;
    // rotation
    // rotation config
    float rotation_duration;
    // rotation status
    bool rotating;
    float rotation_radians;
    float rotation_radians_start;
    float rotation_radians_target;
    float rotation_t;
    float rotation_time;
    int rotation_dir; // +1 -1
};

static WorldViewConfig g_config;
static WorldViewTopCamera g_camera_top;
static WorldViewRayCamera g_camera_ray;

void world_view_init() {
    memset(&g_config, 0, sizeof(WorldViewConfig));

    g_config.loot_at = Vec3{ 0, 0, 0 };
    g_config.screen_width = buffer_get_width();
    g_config.screen_height = buffer_get_height();
    g_config.view_type = WorldViewType_Top;
    g_config.view_top_dir = WorldViewTopDir_Front;

    // top
    world_view_top_init();
    world_view_top_init_camera(
        &g_camera_top,
        g_config.loot_at,
        g_config.screen_width,
        g_config.screen_height,
        g_config.view_top_dir
    );
}

void world_view_set_focus(Vec3 look_at) {
    g_config.loot_at = look_at;
    // TOP
    world_view_top_set_focus(&g_camera_top, look_at);
}

// rotation
bool world_view_if_rotating() {
    return g_config.rotating;
}

void world_view_start_rotation(int direction) {
    if (g_config.rotating) {
        return;
    }
    g_config.rotation_dir = direction;
    g_config.rotation_time = 0;
    g_config.rotation_duration = timer_get_rhythm_step() * 1.5f; // TODO
    g_config.rotation_radians_start = world_view_top_get_rotation_radians(g_config.view_top_dir);
    g_config.rotation_radians_target = g_config.rotation_radians_start + direction * PI_HALF;
    g_config.view_type = WorldViewType_Ray;
    g_config.rotating = true;
}

void world_view_end_rotation() {
    g_config.view_top_dir = (WorldViewTopDir)((g_config.view_top_dir + g_config.rotation_dir + 4) % 4);
    world_view_top_set_dir(&g_camera_top, g_config.view_top_dir);
    g_config.view_type = WorldViewType_Top;
    g_config.rotating = false;
}

void world_view_update() {
    if (!g_config.rotating) {
        return;
    }
    g_config.rotation_time += timer_get_delta_time();
    g_config.rotation_t = clamp(0.0f, g_config.rotation_time / g_config.rotation_duration, 1.0f);
    if (g_config.rotation_t >= 1.0f) {
        world_view_end_rotation();
        return;
    }
    float t = g_config.rotation_t;
    // t = t < 0.5 ? 2 * t * t : 1 - powf(-2 * t + 2, 2) / 2; // easeInOutQuad
    t = t < 0.5 ? 8 * t * t * t * t : 1 - powf(-2 * t + 2, 4) / 2; // easeInOutQuart
    g_config.rotation_radians = lerp(g_config.rotation_radians_start, g_config.rotation_radians_target, t);
    // update rotation
    world_view_ray_init_camera(
        &g_camera_ray,
        g_config.loot_at,
        g_config.screen_width,
        g_config.screen_height,
        g_config.rotation_radians
    );
}

void world_view_draw() {
    // TODO buffer
    switch (g_config.view_type) {
    case WorldViewType_Top: {
        world_view_top_draw(&g_camera_top);
        break;
    }
    case WorldViewType_Ray: {
        world_view_ray_draw(&g_camera_ray);
        break;
    }
    }
}

// top with current camera
WorldViewTopCamera* world_view_get_camera_top() {
    return &g_camera_top;
}

Vec3 world_view_get_forward() {
    return world_view_top_get_forward(g_config.view_top_dir);
}

Vec3 world_view_get_right() {
    return world_view_top_get_right(g_config.view_top_dir);
}

// ray-cast camera
WorldViewRayCamera* world_view_get_camera_ray() {
    return &g_camera_ray;
}