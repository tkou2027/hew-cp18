#pragma once
#include "math.h"

namespace hoge
{
	class Vec3 {
	public:
		enum class Axis { X, Y, Z };

		Vec3() = default;
		Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

		Vec3 operator+(const Vec3& rhs) const
		{
			return Vec3{ x + rhs.x, y + rhs.y, z + rhs.z };
		}

		Vec3 operator-(const Vec3& rhs) const
		{
			return Vec3{ x - rhs.x, y - rhs.y, z - rhs.z };
		}

		Vec3 operator*(float scalar) const
		{
			return Vec3{ x * scalar, y * scalar, z * scalar };
		}

		bool operator==(const Vec3& rhs) const
		{
			return std::fabs(x - rhs.x) < Math::EPSILON &&
				std::fabs(y - rhs.y) < Math::EPSILON &&
				std::fabs(z - rhs.z) < Math::EPSILON;
		}

		Vec3 Round() const
		{
			return Vec3{
				std::floor(x),
				std::floor(y),
				std::floor(z)
			};
		}

		float Length() const
		{
			return std::sqrt(x * x + y * y + z * z);
		}

		float LengthSquared() const
		{
			return x * x + y * y + z * z;
		}

		float GetAxis(Axis axis) const
		{
			switch (axis) {
			case Axis::X: return x;
			case Axis::Y: return y;
			case Axis::Z: return z;
			}
			return 0.0f; // fallback
		}

		void SetAxis(Axis axis, float value)
		{
			switch (axis) {
			case Axis::X: x = value; break;
			case Axis::Y: y = value; break;
			case Axis::Z: z = value; break;
			}
		}

		Vec3 RotateY(float sin_theta, float cos_theta) const
		{
			return Vec3{
				cos_theta * x + sin_theta * z,
				y,
				-sin_theta * x + cos_theta * z
			};
		}

		Vec3 RotateReverseY(float sin_theta, float cos_theta) const
		{
			return Vec3{
				cos_theta * x - sin_theta * z,
				y,
				sin_theta * x + cos_theta * z
			};
		}

		Vec3 GetMajorAxis() const {
			float abs_x = std::fabs(x);
			float abs_z = std::fabs(z);
			if (abs_x > abs_z)
			{
				return Vec3{ (x > 0 ? 1.f : -1.f), 0.f, 0.f };
			}
			if (abs_z > abs_x)
			{
				return Vec3{ 0.f, 0.f, (z > 0 ? 1.f : -1.f) };
			}
			return Vec3{ (x > 0 ? 1.f : -1.f), 0.f, (z > 0 ? 1.f : -1.f) };
		}

		float x{ 0.0f };
		float y{ 0.0f };
		float z{ 0.0f };
	};
}