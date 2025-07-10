#pragma once
#include "pixel.h"

struct Sprite {
    Pixel* pixels;
    bool* alpha;
    int height;
    int width;
};

void sprite_init(Sprite* p_sprite, int width, int height);
void sprite_init(Sprite* p_sprite, int width, int height,
    const wchar_t* text, Color color);

void sprite_load(Sprite* p_sprite, const wchar_t* filename);

void sprite_destroy(Sprite* p_sprite);

void sprite_draw_sprite(Coord2 pos, const Sprite* sprite);
void sprite_draw_sprite_alpha(Coord2 pos, const Sprite* sprite);