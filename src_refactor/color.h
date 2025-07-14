#pragma once
#include <stdint.h>

namespace hoge
{
	class Color
	{
	public:
		Color() = default;
		Color(int r, int g, int b);
		static Color Lerp(const Color& a, const Color& b, float t);
		bool operator==(const Color& rhs) const;

		uint8_t m_r{ 0 };
		uint8_t m_g{ 0 };
		uint8_t m_b{ 0 };
	};
}