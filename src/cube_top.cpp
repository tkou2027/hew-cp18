#include "cube_top.h"
#include "cube.h"
#include "draw.h"
#include "world_view.h"

int g_cube_scale_width = 4;
int g_cube_scale_height = 1;
int g_cube_scale_depth = g_cube_scale_height * 2;

float cube_top_get_scale_width() {
    return g_cube_scale_width;
}

float cube_top_get_scale_height() {
    return g_cube_scale_height;
}

float cube_top_get_scale_depth() {
    return g_cube_scale_depth;
}

void cube_top_draw_face_top(int offset_x, int offset_y, const BlockInfo* block_info, float depth_z) {
    FaceInfo face_info;
    cube_get_face_info(&face_info, FaceDir_Top, block_info);
    // top shadow
    int x = block_info->pos.x;
    int y = block_info->pos.y;
    int z = block_info->pos.z;
    Vec3 right = world_view_get_right();
    Vec3 forward = world_view_get_forward();
    bool top_shadow = world_get_visible_active_block_at(x + forward.x, y + 1, z + forward.z);
        //|| world_if_block_ground(x - forward.x, y + 1, z - forward.z);
    bool side_shadow = world_get_visible_active_block_at(x - right.x, y + 1, z - right.z);
    //bool side_shadow_right = world_if_block_ground(x + right.x, y + 1, z + right.z);;
    bool corner_shadow = world_get_visible_active_block_at(x + forward.x - right.x, y + 1, z + forward.z - right.z);
    bool fall_shadow = world_get_visible_block_at(x, y + 1, z);
    for (int y = 0; y < g_cube_scale_depth; ++y) {
        for (int x = 0; x < g_cube_scale_width; ++x) {
            Pixel pixel = (y == 0 && top_shadow
                || x <= 1 && side_shadow
                || y == 0 && x <= 1 && corner_shadow
                || fall_shadow) ?
                face_info.shadow_pixels[0] : face_info.pixels[0];
            //Pixel pixel = face_info.pixels[0];
            if (face_info.has_pixel_mix) {
                pixel = pixel_get_mixed(&pixel, &face_info.pixel_mix);
            }
            draw_pixel(offset_x + x, offset_y + y, pixel);// pixel_top);
            buffer_z_set_at(offset_x + x, offset_y + y, depth_z); // z-buffer
        }
    }
}

void cube_top_draw_face_front(int offset_x, int offset_y, const BlockInfo* block_info, float depth_z) {
    FaceInfo face_info;
    cube_get_face_info(&face_info, FaceDir_Front, block_info);
    // Pixel pixel_front = create_pixel(cube->color_front);
    for (int x = 0; x < g_cube_scale_width; ++x) {
        draw_pixel(offset_x + x, offset_y + 2, face_info.pixels[x % 2]); // pixel_front);
        buffer_z_set_at(offset_x + x, offset_y + 2, depth_z + 1); // z-buffer
    }
}

void cube_top_draw(int offset_x, int offset_y, const BlockInfo* block_info, float z) {
    cube_top_draw_face_top(offset_x, offset_y, block_info, z);
    cube_top_draw_face_front(offset_x, offset_y, block_info, z);
}