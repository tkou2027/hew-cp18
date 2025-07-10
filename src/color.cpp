
#include "color.h"
// color
bool if_color_equal(Color c1, Color c2) {
	return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

Color create_color(int r, int g, int b) {
	uint8_t rClamped = clamp(0, r, 255);
	uint8_t gClamped = clamp(0, g, 255);
	uint8_t bClamped = clamp(0, b, 255);
	return Color{ rClamped, gClamped, bClamped };
}

Color create_color(int colorCode) {
	switch (colorCode) {
	case ColorPreset_White:
		return Color{ 255, 255, 255 };
	case ColorPreset_Black:
		return Color{ 0, 0, 0 };
	}
}

Color create_color(Vec3 v) {
	return create_color(v.x * 255, v.y * 255, v.z * 255);
}

Color lerp(Color a, Color b, float t) {
	return create_color(
		lerp(a.r, b.r, t),
		lerp(a.g, b.g, t),
		lerp(a.b, b.b, t)
	);
}

Color color_shade(Color c, float t) {
    return create_color(
        c.r * t,
        c.g * t,
        c.b * t
    );
}