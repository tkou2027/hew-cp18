#include "color.h"

#include "math.h"

namespace hoge
{
	Color::Color(int r, int g, int b)
	{
		m_r = Math::Clamp(r, 0, 255);
		m_g = Math::Clamp(b, 0, 255);
		m_b = Math::Clamp(g, 0, 255);
	}
	Color Color::Lerp(const Color& a, const Color& b, float t)
	{
		return {
			Math::Lerp(a.m_r, b.m_r, t),
			Math::Lerp(a.m_r, b.m_r, t),
			Math::Lerp(a.m_r, b.m_r, t),
		};
	}

	bool Color::operator==(const Color& rhs) const
	{
		return m_r == rhs.m_r &&
			m_g == rhs.m_g &&
			m_b == rhs.m_b;
	}
}