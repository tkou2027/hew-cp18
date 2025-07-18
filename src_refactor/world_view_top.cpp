#include "world_view_top.h"
#include "cube_top.h"

namespace hoge
{
	void WorldViewTop::Initialize(std::weak_ptr<World> world)
	{
		m_cube_scale_height = CubeTop::SCALE_HEIGHT;
		m_cube_scale_width = CubeTop::SCALE_WIDTH;
		m_cube_scale_depth = CubeTop::SCALE_DEPTH;
		m_world = world;
		if (auto world_p = m_world.lock())
		{
			m_world_height = world_p->GetHeight();
			m_world_width = world_p->GetWidth();
			m_world_depth = world_p->GetDepth();
		}
		// TODO: error
	}
	void WorldViewTop::InitializeCamera()
	{
	}
	void WorldViewTop::Draw(const WorldViewTopCamera& camera)
	{
		World::WorldBoundary boundary{};
		if (auto world_p = m_world.lock())
		{
			boundary = world_p->GetBoundary();
		}
		// TODO: error
		switch (camera.dir) {
		case WorldViewTopDir::FRONT:
		case WorldViewTopDir::LEFT: {
			for (int y = boundary.min_y; y < boundary.max_y; ++y) {
				for (int z = boundary.max_z - 1; z >= boundary.min_z; --z) {
					for (int x = boundary.max_x - 1; x >= boundary.min_x; --x) {
						DrawCube(camera, x, y, z);
					}
				}
			}
			break;
		}
		case WorldViewTopDir::RIGHT:
		case WorldViewTopDir::BACK: {
			for (int y = boundary.min_y; y < boundary.max_y; ++y) {
				for (int z = boundary.min_z; z < boundary.max_z; ++z) {
					for (int x = boundary.min_x; x < boundary.max_x; ++x) {
						DrawCube(camera, x, y, z);
					}
				}
			}
			break;
		}
		}
	}

	void WorldViewTop::DrawCube(const WorldViewTopCamera& camera, float x, float y, float z)
	{
		auto world_p = m_world.lock();
		if (!world_p)
		{
			return;
			// TODO: error
		}
		BlockInfo block_info;
		if (!world_p->GetVisibleBlockInfoAt(x, y, z, block_info)) {
			return;
		}
		BlockInfo block_info_tmp;
		bool top_visible = !world_p->GetVisibleBlockInfoAt(x, y + 1, z, block_info_tmp);
		Vec3 front_dir = GetForward(camera.dir);
		bool front_visible = !world_p->GetVisibleBlockInfoAt(x - front_dir.x, y, z - front_dir.z, block_info_tmp);
		if (!top_visible && !front_visible) {
			return;
		}
		int offset_y = 0;
		if (block_info.block->status == BlockStatus::FALL) {
			offset_y = block_info.block->offset_y;
		}
		Vec2 draw_offset = GetCubeOffset(camera, x, y + offset_y, z);

		float depth_z = GetZValue(camera.dir, x, z);
		if (top_visible) {
			CubeTop::DrawFaceTop(draw_offset.x, draw_offset.y, block_info, depth_z);
		}
		if (front_visible) {
			CubeTop::DrawFaceFront(draw_offset.x, draw_offset.y, block_info, depth_z);
		}
	}

	void WorldViewTop::SetFocus()
	{
	}
	void WorldViewTop::SetDir()
	{
	}
	Vec3 WorldViewTop::GetRight(WorldViewTopDir dir) const
	{
		return Vec3();
	}
	Vec3 WorldViewTop::GetForward(WorldViewTopDir dir) const
	{
		return Vec3();
	}
	float WorldViewTop::GetRotationRadians(WorldViewTopDir dir) const
	{
		return 0.0f;
	}
	float WorldViewTop::GetZValue(WorldViewTopDir dir, float z, float) const
	{
		return 0.0f;
	}
	Vec3 WorldViewTop::GetWorldFromScreen(const WorldViewTopCamera& camera, Vec2 screen_pos, float world_y)
	{
		return Vec3();
	}
	Vec2 WorldViewTop::GetScreenFromWorld(const WorldViewTopCamera& camera, Vec3 world_pos)
	{
		return Vec2();
	}
	Vec2 WorldViewTop::GetCubeOffset(const WorldViewTopCamera& camera, float x, float y, float z)
	{
		return Vec2();
	}
}

