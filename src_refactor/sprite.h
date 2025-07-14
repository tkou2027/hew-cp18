#pragma once

#include <string>

#include "pixel.h"

namespace hoge
{
	class Sprite
	{
	public:
		~Sprite();
		void Initialize(int width, int height);
		void Load(const std::wstring& filename);
		void Draw(int x, int y) const;
	private:
		int m_width{ 0 };
		int m_height{ 0 };
		Pixel* m_pixels{ nullptr };
	};
}