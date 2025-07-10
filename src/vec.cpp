#include "vec.h"
#include <cmath>
// math uitls
static float epsilion = 1e-5;

// 1D //
//float abs(float x) {
//	return x >= 0 ? x : -x;
//}

float lerp(float a, float b, float t) {
    return a * (1 - t) + b * t;
}

float if_zero(float x) {
    return x > -epsilion && x < epsilion;
}

float round(float x, float t) {
    if (if_zero(t)) {
        return x;
    }
    float t_inv = 1.0f / t;
    return (float)((int)(x * t_inv + 0.5)) * t;
}

// round to 0.5
float round_half(float x) {
    return roundf(x * 2.0f) / 2.0f;
}

float sign(float x) {
    return x > epsilion ? 1 : (x < -epsilion ? -1 : 0);
}

float clamp(float min, float x, float max) {
    return x < min ? min : (x > max ? max : x);
}

int clamp(int min, int x, int max) {
    return x < min ? min : (x > max ? max : x);
}

// 2D //
Vec2 vec2_add(Vec2 a, Vec2 b) {
    return Vec2{ a.x + b.x, a.y + b.y };
}

Vec2 vec2_sub(Vec2 a, Vec2 b) {
    return Vec2{ a.x - b.x, a.y - b.y };
}

Vec2 vec2_mult(Vec2 v, float f) {
    return Vec2{ v.x * f, v.y * f };
}

float vec2_dist(Vec2 a, Vec2 b) {
    Vec2 dist{ a.x - b.x, a.y - b.y };
    return std::sqrtf(dist.x * dist.x + dist.y * dist.y);
}

float vec2_dist_squared(Vec2 a, Vec2 b) {
    Vec2 dist{ a.x - b.x, a.y - b.y };
    return dist.x * dist.x + dist.y * dist.y;
}

float vec2_length(Vec2 v) {
    return std::sqrtf(v.x * v.x + v.y * v.y);
}

float vec2_length_squared(const Vec2* v) {
    return v->x * v->x + v->y * v->y;
}

int coord2_distance_squared(const Coord2* a, const Coord2* b) {
    int x = a->x - b->x;
    int y = (a->y - b->y) * 2;
    return x * x + y * y;
}

Coord2 coord2_floor(const Vec2 v) {
    return Coord2{ (int)v.x, (int)v.y };
}

Coord2 coord2_round(const Vec2 v) {
    return Coord2{ (int)(v.x + 0.5f), (int)(v.y + 0.5f) };
}


// 3D //
Vec3 vec3_multiply(const Vec3 v, float t) {
    return Vec3{ v.x * t, v.y * t, v.z * t };
}

Vec3 vec3_multiply(const Vec3* v, float t) {
    return Vec3{ v->x * t, v->y * t, v->z * t };
}

Vec3 vec3_add(const Vec3 v, float t) {
    return Vec3{ v.x + t, v.y + t, v.z + t };
}

Vec3 vec3_add(const Vec3* v, float t) {
    return Vec3{ v->x + t, v->y + t, v->z + t };
}

Vec3 vec3_add(const Vec3 v1, const Vec3 v2) {
    return Vec3{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

Vec3 vec3_add(const Vec3* v1, const Vec3* v2) {
    return Vec3{ v1->x + v2->x, v1->y + v2->y, v1->z + v2->z };
}

Vec3 vec3_sub(const Vec3 v1, const Vec3 v2) {
    return Vec3{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

Vec3 vec3_sub(const Vec3* v1, const Vec3* v2) {
    return Vec3{ v1->x - v2->x, v1->y - v2->y, v1->z - v2->z };
}

Vec3 vec3_round(const Vec3* v) {
    return Vec3{ floorf(v->x), floorf(v->y), floorf(v->z) };
}

Vec3 vec3_round(const Vec3 v) {
    return Vec3{ floorf(v.x), floorf(v.y), floorf(v.z) };
}

bool vec3_if_equal(const Vec3* v1, const Vec3* v2) {
    return fabs(v1->x - v2->x) < epsilion
        && fabs(v1->y - v2->y) < epsilion
        && fabs(v1->z - v2->z) < epsilion;
}

bool vec3_if_equal(const Vec3 v1, const Vec3 v2) {
    return fabs(v1.x - v2.x) < epsilion
        && fabs(v1.y - v2.y) < epsilion
        && fabs(v1.z - v2.z) < epsilion;
}

float vec3_get_axis(const Vec3* v, Vec3Axis axis) {
    switch (axis) {
    case Vec3Axis_X: {
        return v->x;
    }
    case Vec3Axis_Y: {
        return v->y;
    }
    case Vec3Axis_Z: {
        return v->z;
    }
    }
}

void vec3_set_axis(Vec3* v, Vec3Axis axis, float value) {
    switch (axis) {
    case Vec3Axis_X: {
        v->x = value;
        break;
    }
    case Vec3Axis_Y: {
        v->y = value;
        break;
    }
    case Vec3Axis_Z: {
        v->z = value;
        break;
    }
    }
}

Vec3 vec3_rotate_y(const Vec3* v, float sin_theta, float cos_theta) {
    return Vec3{
        cos_theta * v->x + sin_theta * v->z,
        v->y,
        -sin_theta * v->x + cos_theta * v->z
    };
}

Vec3 vec3_rotate_reverse_y(const Vec3* v, float sin_theta, float cos_theta) {
    return Vec3{
        cos_theta * v->x - sin_theta * v->z,
        v->y,
        sin_theta * v->x + cos_theta * v->z
    };
}

Vec3 vec3_get_major_axis(const Vec3* v) {
	float abs_x = fabs(v->x);
	float abs_z = fabs(v->z);
	if (abs_x > abs_z) {
		return Vec3{ sign(v->x), 0, 0 };
	}
	else if (abs_x < abs_z) {
		return Vec3{ 0, 0, sign(v->z) };
	}
	else {
		return Vec3{ sign(v->x), 0, sign(v->z) };
	}
}