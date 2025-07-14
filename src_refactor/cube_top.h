#pragma once
#include "block.h"

namespace hoge
{
	class CubeTop
	{
	public:
		static constexpr int SCALE_WIDTH{ 4 };
		static constexpr int SCALE_HEIGHT{ 1 };
		static constexpr int SCALE_DEPTH{ SCALE_HEIGHT * 2 };

		static void DrawFaceTop(int offset_x, int offset_y, const BlockInfo& block, float z);
		static void DrawFaceFront(int offset_x, int offset_y, const BlockInfo& block, float z);
	};
}