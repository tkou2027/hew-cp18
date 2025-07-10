#pragma once
#include "cube.h"
#include "world.h"

float cube_top_get_scale_width();
float cube_top_get_scale_height();
float cube_top_get_scale_depth();
void cube_top_draw_face_top(int offset_x, int offset_y, const BlockInfo* block_info, float z);
void cube_top_draw_face_front(int offset_x, int offset_y, const BlockInfo* block_info, float z);
void cube_top_draw(int offset_x, int offset_y, const BlockInfo* block_info, float z);