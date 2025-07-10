#pragma once
#include "vec.h"
#include "color.h"
#include "pixel.h"
#include "sprite.h"

int buffer_get_width();
int buffer_get_height();
int buffer_get_size();

void buffer_init(int width, int height);
void buffer_draw_update();
void buffer_draw_swap();
void buffer_clear();
void buffer_destroy();
Pixel* buffer_get_pixel_at(int x, int y);
Pixel* buffer_get_pixel_at(Coord2 pos);
// z-buffer
float* buffer_get_z_buffer();
float buffer_z_get_at(int x, int y);
void buffer_z_set_at(int x, int y, float value);
void buffer_z_clear();

void draw_pixel(int x, int y, Pixel pixel);
void draw_pixel(Coord2 pos, Pixel pixel);
void draw_pixel_alpha(int x, int y, Pixel pixel);
void draw_pixel_alpha(Coord2 pos, Pixel pixel);
void draw_fill(Coord2 pos, Pixel pixel, int width, int height);
void draw_line(Coord2 pos1, Coord2 pos2, Pixel pixel, bool reverse = false);
void draw_sprite(Coord2 pos, const Sprite* sprite);
void draw_sprite_alpha(Coord2 pos, const Sprite* sprite);
void draw_color(Coord2 pos, Color color);
void draw_sprite_color(Coord2 pos, const Sprite* sprite);
void draw_string(int x, int y, const wchar_t* s, Color color_fore, Color color_back);
void draw_string_fullwidth(int x, int y, const wchar_t* s, Color color_fore, Color color_back);
#ifdef _DEBUG
// =============================================================================
// デバッグ表示
// =============================================================================
void DispFPSBuffer(int fpsCounter);
#endif