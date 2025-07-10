#pragma once
#include "pixel.h"
#include "world.h"

// face
enum FaceDir {
    FaceDir_Top,
    FaceDir_Front,
    FaceDir_Left,
    FaceDir_right,
    FaceDir_Back,
    FaceDir_Bottom
};

enum FacePattern {
    FacePattern_Solid,
    FacePattern_Stripe
};

struct FaceInfo {
    FaceDir dir;
    FacePattern pattern;
    Color* colors;
    Pixel* pixels;
    Color* shadow_colors;
    Pixel* shadow_pixels;
    bool has_pixel_mix;
    Pixel pixel_mix;
};

// cube
struct Cube {
    BlockViewType type; // BlockViewType
    int face_types[6];
};

void cube_get_face_info(
    FaceInfo* face_info, // out
    FaceDir face_dir,
    const BlockInfo* block_info
);

Color cube_get_top_color(const BlockInfo* block_info);
Color cube_get_shadow_color(const BlockInfo* block_info, float shade = 0.8f); // for shadow