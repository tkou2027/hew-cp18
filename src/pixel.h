#pragma once
#include "color.h"

struct Pixel {
    Color color_fore;
    Color color_back;
    wchar_t text;
    bool fullwidth;
};

Pixel create_pixel(Color color);
Pixel create_pixel(wchar_t text, Color color_fore);
Pixel create_pixel(wchar_t text, Color color_fore, Color color_back);
Pixel create_pixel_fullwidth();
Pixel pixel_get_mixed(const Pixel* p1, const Pixel* p2);
