#pragma once
#include "timer.h"
#include "color.h"
#include "draw.h"
inline void draw_animation_test() {
    int frames = 4;
    Color colors[] = {
        create_color(255, 255, 255),
        create_color(255, 0, 0),
        create_color(0, 255, 0),
        create_color(0, 0, 255)
    };
    int pos = timer_get_rhythm_position();
    int frame = pos % frames;
    draw_pixel(70, 22, create_pixel(colors[frame]));
}