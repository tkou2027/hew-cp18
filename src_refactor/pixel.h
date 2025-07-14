#pragma once

#include "color.h"

namespace hoge
{
	class Pixel
	{
	public:
		// to show fullwidth text, skip next pixel when drawing
		static Pixel CreateSkipped()
		{
			Pixel p{};
			p.m_skip = true;
			return p;
		}

		Pixel() = default;
		Pixel(const Color& color)
			: m_color_fore(color), m_color_back(color) {}
		Pixel(wchar_t text, const Color& color_fore)
			: m_text{ text }, m_color_fore(color_fore) {}
		Pixel(wchar_t text, const Color& color_fore, const Color& color_back)
			: m_text{ text }, m_color_fore(color_fore), m_color_back(color_back) {}

		bool IfSkipped() const
		{
			return m_skip;
		}
		bool operator==(const Pixel& rhs) const
		{
			return m_color_fore == rhs.m_color_fore &&
				m_color_back == rhs.m_color_back &&
				m_text == rhs.m_text &&
				m_skip == rhs.m_skip;
		}

		Color m_color_fore{};
		Color m_color_back{};
		wchar_t m_text{ L' ' };
		bool m_skip{ false };
	};
}