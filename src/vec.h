#pragma once
// math uitls
// TODO DOOO
#define PI (3.142857f)
#define PI_HALF (1.5714285f)

// 1D //
//float abs(float x);
float if_zero(float x);
float lerp(float a, float b, float t);
float clamp(float min, float x, float max);
float round(float x, float step);
float round_half(float x);
float sign(float x);
int clamp(int min, int x, int max);

// 2D //
struct Coord2 {
    int x;
    int y;
};

struct Vec2 {
    float x;
    float y;
};

int coord2_distance_squared(const Coord2* a, const Coord2* b);
Coord2 coord2_floor(const Vec2 v);
Coord2 coord2_round(const Vec2 v);
float vec2_length_squared(const Vec2* v);
Vec2 vec2_add(Vec2 a, Vec2 b);
Vec2 vec2_sub(Vec2 a, Vec2 b);
Vec2 vec2_mult(Vec2 v, float f);
float vec2_dist(Vec2 a, Vec2 b);
float vec2_dist_squared(Vec2 a, Vec2 b);
float vec2_length(Vec2 v);

// 3D //
enum Vec3Axis {
    Vec3Axis_X,
    Vec3Axis_Y,
    Vec3Axis_Z
};

struct Vec3 {
    float x;
    float y;
    float z;
};

struct Vec3Boundary {
	Vec3 min;
	Vec3 max;
};

struct Vec3IntBoundary {
	int min_x;
	int min_y;
	int min_z;
	int max_x;
	int max_y;
	int max_z;
};

Vec3 vec3_multiply(const Vec3 v, float t);
Vec3 vec3_multiply(const Vec3* v, float t);
Vec3 vec3_add(const Vec3 v, float t);
Vec3 vec3_add(const Vec3* v, float t);
Vec3 vec3_add(const Vec3 v1, const Vec3 v2);
Vec3 vec3_add(const Vec3* v1, const Vec3* v2);
Vec3 vec3_sub(const Vec3 v1, const Vec3 v2);
Vec3 vec3_sub(const Vec3* v1, const Vec3* v2);
Vec3 vec3_round(const Vec3* v);
Vec3 vec3_round(const Vec3 v);
bool vec3_if_equal(const Vec3* v1, const Vec3* v2);
bool vec3_if_equal(const Vec3 v1, const Vec3 v2);
float vec3_get_axis(const Vec3* v, Vec3Axis axis);
void vec3_set_axis(Vec3* v, Vec3Axis axis, float value);
Vec3 vec3_rotate_y(const Vec3* v, float sin_theta, float cos_theta);
Vec3 vec3_rotate_reverse_y(const Vec3* v, float sin_theta, float cos_theta);
Vec3 vec3_get_major_axis(const Vec3* v);