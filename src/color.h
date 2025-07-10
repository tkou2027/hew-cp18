#pragma once
#include <stdint.h>
#include "vec.h"

// color
enum ColorPreset {
	ColorPreset_Black,
	ColorPreset_White
};

struct Color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

bool if_color_equal(Color c1, Color c2);

Color create_color(int r, int g, int b);

Color create_color(int colorCode);

Color create_color(Vec3 v);
Color lerp(Color a, Color b, float t);
Color color_shade(Color c, float t);