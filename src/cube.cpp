#include "cube.h"
#include "color.h"
#include "pixel.h"
#include "timer.h"

// definitions of the many cubes
Color ground_top_colors[] = {
    // level 0
    create_color(3, 0, 124),
    create_color(5, 0, 141),
    // level 1
    create_color(63, 50, 162),
    create_color(85, 74, 169),
    // level 2
    //create_color(96, 92, 180),
    //create_color(111, 108, 183),
    // level 3
    create_color(107, 117, 186),
    create_color(134, 143, 201),
    // level 4
    create_color(123, 156, 199),
    create_color(152, 180, 216),
    // level 5
    create_color(142, 193, 212),
    create_color(173, 216, 231),
    // level 6
    create_color(211, 241, 241),
    create_color(192, 230, 226),
    //// level 0
    //create_color(1, 2, 94),
    //create_color(6, 0, 141),
    //// level 1
    //create_color(58, 41, 156),
    //create_color(85, 74, 169),
    //// level 2
    //create_color(111, 108, 184),
    //create_color(132, 142, 201),
    //// level 3
    //create_color(153, 180, 215),
    //create_color(173, 216, 231),
};

float side_shade = 0.4f;
Color ground_side_colors[] = {
    color_shade(ground_top_colors[0], side_shade),
    color_shade(ground_top_colors[1], side_shade),
    color_shade(ground_top_colors[2], side_shade),
    color_shade(ground_top_colors[3], side_shade),
    color_shade(ground_top_colors[4], side_shade),
    color_shade(ground_top_colors[5], side_shade),
    color_shade(ground_top_colors[6], side_shade),
    color_shade(ground_top_colors[7], side_shade),
    color_shade(ground_top_colors[8], side_shade),
    color_shade(ground_top_colors[9], side_shade),
    color_shade(ground_top_colors[10], side_shade),
    color_shade(ground_top_colors[11], side_shade),
    //color_shade(ground_top_colors[12], side_shade),
    //color_shade(ground_top_colors[13], side_shade)
    /*
    // level 0
    create_color(6, 0, 80),
    create_color(14, 8, 86),
    // level 1
    create_color(40, 44, 101),
    create_color(73, 78, 126),
    // level 2
    create_color(104, 125, 152),
    create_color(112, 153, 158),
    */
    // create_color(11, 16, 19),
    // create_color(14, 37, 65),
};

float top_shadow_shade = 0.6f;
Color ground_top_shadow_colors[] = {
    color_shade(ground_top_colors[0], top_shadow_shade),
    color_shade(ground_top_colors[1], top_shadow_shade),
    color_shade(ground_top_colors[2], top_shadow_shade),
    color_shade(ground_top_colors[3], top_shadow_shade),
    color_shade(ground_top_colors[4], top_shadow_shade),
    color_shade(ground_top_colors[5], top_shadow_shade),
    color_shade(ground_top_colors[6], top_shadow_shade),
    color_shade(ground_top_colors[7], top_shadow_shade),
    color_shade(ground_top_colors[8], top_shadow_shade),
    color_shade(ground_top_colors[9], top_shadow_shade),
    color_shade(ground_top_colors[10], top_shadow_shade),
    color_shade(ground_top_colors[11], top_shadow_shade),
    //color_shade(ground_top_colors[12], top_shadow_shade),
    //color_shade(ground_top_colors[13], top_shadow_shade)
};

Pixel ground_top_pixels[] = {
    create_pixel(ground_top_colors[0]),
    create_pixel(ground_top_colors[1]),
    create_pixel(ground_top_colors[2]),
    create_pixel(ground_top_colors[3]),
    create_pixel(ground_top_colors[4]),
    create_pixel(ground_top_colors[5]),
    create_pixel(ground_top_colors[6]),
    create_pixel(ground_top_colors[7]),
    create_pixel(ground_top_colors[8]),
    create_pixel(ground_top_colors[9]),
    create_pixel(ground_top_colors[10]),
    create_pixel(ground_top_colors[11]),
    //create_pixel(ground_top_colors[12]),
    //create_pixel(ground_top_colors[13]),
};
Pixel ground_side_pixels[] = {
    create_pixel(L'\u2590', ground_side_colors[0], ground_side_colors[1]),
    create_pixel(L'\u2590', ground_side_colors[0], ground_side_colors[1]),
    create_pixel(L'\u2590', ground_side_colors[2], ground_side_colors[3]),
    create_pixel(L'\u2590', ground_side_colors[2], ground_side_colors[3]),
    create_pixel(L'\u2590', ground_side_colors[4], ground_side_colors[5]),
    create_pixel(L'\u2590', ground_side_colors[4], ground_side_colors[5]),
    create_pixel(L'\u2590', ground_side_colors[6], ground_side_colors[7]),
    create_pixel(L'\u2590', ground_side_colors[6], ground_side_colors[7]),
    create_pixel(L'\u2590', ground_side_colors[8], ground_side_colors[9]),
    create_pixel(L'\u2590', ground_side_colors[8], ground_side_colors[9]),
    create_pixel(L'\u2590', ground_side_colors[10], ground_side_colors[11]),
    create_pixel(L'\u2590', ground_side_colors[10], ground_side_colors[11]),
    //create_pixel(L'\u2590', ground_side_colors[12], ground_side_colors[13]),
    //create_pixel(L'\u2590', ground_side_colors[12], ground_side_colors[13])
    //create_pixel(ground_side_colors[0]),
    //create_pixel(ground_side_colors[1]),
    //create_pixel(ground_side_colors[2]),
    //create_pixel(ground_side_colors[3]),
    //create_pixel(ground_side_colors[4]),
    //create_pixel(ground_side_colors[5]),
    //create_pixel(ground_side_colors[6]),
    //create_pixel(ground_side_colors[7])
};
Pixel ground_top_shadow_pixels[] = {
    create_pixel(ground_top_shadow_colors[0]),
    create_pixel(ground_top_shadow_colors[1]),
    create_pixel(ground_top_shadow_colors[2]),
    create_pixel(ground_top_shadow_colors[3]),
    create_pixel(ground_top_shadow_colors[4]),
    create_pixel(ground_top_shadow_colors[5]),
    create_pixel(ground_top_shadow_colors[6]),
    create_pixel(ground_top_shadow_colors[7]),
    create_pixel(ground_top_shadow_colors[8]),
    create_pixel(ground_top_shadow_colors[9]),
    create_pixel(ground_top_shadow_colors[10]),
    create_pixel(ground_top_shadow_colors[11]),
    //create_pixel(ground_top_shadow_colors[12]),
    //create_pixel(ground_top_shadow_colors[13])
};

// end

// face definitions
void cube_get_face_info_ground_top(
    FaceInfo* face_info, // out
    const BlockInfo* block_info// Vec3 world_pos // TODO might have other params
) {
    face_info->pattern = FacePattern_Solid;
    int height = (int)block_info->pos.y % 6;
    int checker = ((int)block_info->pos.x + (int)block_info->pos.z) % 2;
    int index = height * 2 + checker;
    face_info->pixels = ground_top_pixels + index;
    face_info->shadow_pixels = ground_top_shadow_pixels + index;
    face_info->colors = ground_top_colors + index;
    face_info->shadow_colors = ground_top_shadow_colors + index;

    if (block_info->block->is_attacked) {
        face_info->has_pixel_mix = true;
        face_info->pixel_mix = *block_info->block->attack_pixel;
    }
    else if (block_info->block->is_ending) {
        face_info->has_pixel_mix = true;
        face_info->pixel_mix = *block_info->block->ending_pixel;
    }
    else {
        face_info->has_pixel_mix = false;
    }
}

void cube_get_face_info_ground_side(
    FaceInfo* face_info, // out
    const BlockInfo *block_info// Vec3 world_pos // TODO might have other params
) {
    face_info->pattern = FacePattern_Stripe;
    int height = (int)block_info->pos.y % 6;
    int index = height * 2;
    face_info->pixels = ground_side_pixels + index;
    face_info->colors = ground_side_colors + index;
}

// cube definitions
void cube_get_face_info_ground(FaceInfo* face_info, // out
    FaceDir face_dir,
    const BlockInfo *block_info// Vec3 world_pos // TODO might have other params
) {
    face_info->dir = face_dir;
    switch (face_dir) {
    case FaceDir_Top:
        cube_get_face_info_ground_top(face_info, block_info);
        break;
    default:
        cube_get_face_info_ground_side(face_info, block_info);
        break;
    }
}

// public
//void cube_get_face_info(
//    FaceInfo* face_info, // out
//    BlockViewType cube_type,
//    FaceDir face_dir,
//    Vec3 world_pos // TODO might have other params
//) {
//    switch (cube_type) {
//    case BlockViewType_Ground:
//        cube_get_face_info_ground(face_info, face_dir, world_pos);
//        break;
//    default:
//        break;
//    }
//}

void cube_get_face_info(
    FaceInfo* face_info, // out
    FaceDir face_dir,
    const BlockInfo* block_info
) {
    BlockViewType cube_type = block_info->block_prototype->view_type;
    switch (cube_type) {
    case BlockViewType_Ground:
        cube_get_face_info_ground(face_info, face_dir, block_info);
        break;
    default:
        break;
    }
}

Color cube_get_top_color(const BlockInfo* block_info) {
    FaceInfo face_info;
    // cube_get_face_info(&face_info, block_info->block_prototype->view_type, FaceDir_Top, block_info->pos);
    cube_get_face_info(&face_info, FaceDir_Top, block_info);
    return face_info.colors[0];
}

Color cube_get_shadow_color(const BlockInfo* block_info, float shade) {
    FaceInfo face_info;
    // cube_get_face_info(&face_info, block_info->block_prototype->view_type, FaceDir_Top, block_info->pos);
    cube_get_face_info(&face_info, FaceDir_Top, block_info);
    return color_shade(face_info.shadow_colors[0], shade);
}
// public end