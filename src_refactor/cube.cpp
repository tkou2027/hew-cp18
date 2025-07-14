#include "cube.h"
#include "block.h"
// cube color defs
namespace
{
	using namespace hoge;
	Color ground_top_colors[]{
		// level 0
		{3, 0, 124},
		{5, 0, 141},
		// level 1
		{63, 50, 162},
		{85, 74, 169},
		// level 2
		{107, 117, 186},
		{134, 143, 201},
		// level 3
		{123, 156, 199},
		{152, 180, 216},
		// level 4
		{142, 193, 212},
		{173, 216, 231},
		// level 5
		{211, 241, 241},
		{192, 230, 226}
	};
	constexpr float side_shade{ 0.4f };
	constexpr float top_shadow_shade{ 0.6f };
}

namespace hoge
{
	void Cube::GetFaceInfo(FaceDir face_dir, const BlockInfo& block_info, FaceInfo& face_info)
	{
		BlockViewType cube_type = block_info.block_prototype->view_type;
		switch (cube_type) {
		case BlockViewType::GROUND:
			return GetFaceInfoGround(face_dir, block_info, face_info);
			break;
		default:
			break;
		}
	}
	void Cube::GetFaceInfoGround(FaceDir face_dir, const BlockInfo& block_info, FaceInfo& face_info)
	{
		switch (face_info.dir) {
		case FaceDir::TOP:
			GetFaceInfoGroundTop(block_info, face_info);
			break;
		default:
			GetFaceInfoGroundSide(block_info, face_info);
			break;
		}
	}
	void Cube::GetFaceInfoGroundTop(const BlockInfo& block_info, FaceInfo& face_info)
	{
		face_info.pattern = FacePattern::SOLID;
	}
	void Cube::GetFaceInfoGroundSide(const BlockInfo& block_info, FaceInfo& face_info)
	{
		face_info.pattern = FacePattern::STRIPE;
	}
}