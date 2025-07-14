#pragma once
#include <cmath>

namespace hoge
{
    // vector math
    class Vec2 {
    public:
        Vec2() = default;
        Vec2(float x, float y) : x(x), y(y) {}

        Vec2 operator+(const Vec2& rhs) const {
            return Vec2{ x + rhs.x, y + rhs.y };
        }

        Vec2 operator-(const Vec2& rhs) const {
            return Vec2{ x - rhs.x, y - rhs.y };
        }

        Vec2 operator*(float scalar) const {
            return Vec2{ x * scalar, y * scalar };
        }

        float Length() const {
            return std::sqrt(x * x + y * y);
        }

        float LengthSquared() const {
            return x * x + y * y;
        }

        static float distance(const Vec2& a, const Vec2& b) {
            return (a - b).Length();
        }

        static float distanceSquared(const Vec2& a, const Vec2& b) {
            return (a - b).LengthSquared();
        }

        float x{ 0.0f };
        float y{ 0.0f };
    };
}