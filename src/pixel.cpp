#include "pixel.h"
Pixel create_pixel(Color color) {
    return Pixel{
        color,
        color,
        L' ',
        false
    };
};

Pixel create_pixel(wchar_t text, Color color_fore) {
    return Pixel{
        color_fore,
        create_color(0, 0, 0),
        text,
        false
    };
}

Pixel create_pixel(wchar_t text, Color color_fore, Color color_back) {
    return Pixel{
        color_fore,
        color_back,
        text,
        false
    };
};

Pixel create_pixel_fullwidth() {
    return Pixel{
        Color{0, 0, 0},
        Color{0, 0, 0},
        L' ',
        true
    };
}

// TODO
Pixel pixel_get_mixed(const Pixel* p1, const Pixel* p2) {
    float t = p2->text == L' ' ? 0 : (p2->text == L'.' ? 0.3 : (p2->text == L'x' ? 0.5 : 0.8));
    return Pixel{
        create_color(0, 0, 0),
        lerp(p1->color_back, p2->color_fore, t),
        p1->text,
        false
    };
}