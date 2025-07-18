#pragma once
#include "vec2.h"
#include "vec3.h"
#include "world.h"

namespace hoge
{
	class WorldViewTop
	{
	public:
		enum class WorldViewTopDir {
			FRONT,
			RIGHT,
			BACK,
			LEFT
		};
		struct WorldViewTopCamera
		{
			Vec3 look_at{};
			int screen_width{ 0 };
			int screen_height{ 0 };

			WorldViewTopDir dir{ WorldViewTopDir::FRONT };
			// computed
			Vec2 camera_screen_pos{};
			Vec2 camera_offset{};
		};
		// set viewing world
		void Initialize(std::weak_ptr<World> world);
		void InitializeCamera();

		void Draw(const WorldViewTopCamera& camera);
		
		void SetFocus();
		void SetDir();
		// direction
		Vec3 GetRight(WorldViewTopDir dir) const;
		Vec3 GetForward(WorldViewTopDir dir) const;
		float GetRotationRadians(WorldViewTopDir dir) const;
		float GetZValue(WorldViewTopDir dir, float z, float) const;
		// coordinate conversions
		Vec3 GetWorldFromScreen(const WorldViewTopCamera& camera, Vec2 screen_pos, float world_y);
		Vec2 GetScreenFromWorld(const WorldViewTopCamera& camera, Vec3 world_pos);
		Vec2 GetCubeOffset(const WorldViewTopCamera& camera, float x, float y, float z);
	private:
		void DrawCube(
			const WorldViewTopCamera& camera,
			float x, float y, float z
		);

		float m_cube_scale_width{ 0.0f };
		float m_cube_scale_height{ 0.0f };
		float m_cube_scale_depth{ 0.0f };
		float m_world_height{ 0.0f };
		float m_world_width{ 0.0f };
		float m_world_depth{ 0.0f };
		std::weak_ptr<World> m_world;
	};
}