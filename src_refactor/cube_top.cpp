#include "cube_top.h"

#include "cube.h"
#include "world_view_top.h"
#include "global_context.h"
#include "buffer.h"

namespace hoge
{
	void CubeTop::DrawFaceTop(int offset_x, int offset_y, const BlockInfo& block_info, float z)
	{
		Cube::FaceInfo face_info;
		Cube::GetFaceInfo(Cube::FaceDir::TOP, block_info, face_info);
		for (int x = 0; x < SCALE_WIDTH; ++x) {
			g_global_context.m_buffer->DrawPixel(offset_x + x, offset_y + 2, face_info.pixels[x % 2]);
			g_global_context.m_buffer->SetZBuffer(offset_x + x, offset_y + 2, z + 1); // z-buffer
		}
	}
	void CubeTop::DrawFaceFront(int offset_x, int offset_y, const BlockInfo& block_info, float z)
	{
		Cube::FaceInfo face_info;
		Cube::GetFaceInfo(Cube::FaceDir::FRONT, block_info, face_info);
		for (int x = 0; x < SCALE_WIDTH; ++x) {
			g_global_context.m_buffer->DrawPixel(offset_x + x, offset_y + 2, face_info.pixels[x % 2]);
			g_global_context.m_buffer->SetZBuffer(offset_x + x, offset_y + 2, z + 1); // z-buffer
		}
	}
}

