#pragma once

namespace hoge
{
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

    class WorldView
    {

    };
}