#pragma once
#include "vec3.h"
#include "world_view_top.h"

namespace hoge
{
	class WorldView
	{
	public:
		enum class WorldViewType
		{
			TOP, // top-down
			RAY, // ray casting
		};
		struct WorldViewConfig
		{
			// general camera
			Vec3 loot_at{};
			int screen_width{ 0 };
			int screen_height{ 0 };
			// view status
			WorldViewType view_type{ WorldViewType::TOP };
			WorldViewTop::WorldViewTopDir view_top_dir{ WorldViewTop::WorldViewTopDir::FRONT };
			float view_ray_rotation{ 0.0f };
			// rotation
			// rotation config
			float rotation_duration{ 0.0f };
			// rotation status
			bool rotating{ false };
			float rotation_radians{ 0.0f };
			float rotation_radians_start{ 0.0f };
			float rotation_radians_target{ 0.0f };
			float rotation_t{ 0.0f };
			float rotation_time{ 0.0f };
			int rotation_dir{ 0 }; // +1 -1
		};
		void Initialize();
		void Update();
		void Draw();

		void SetFocus(Vec3 look_at);
		void GetForward();
		void GetRight();
		// rotation
		bool IfRotating();
		bool StartRotation(int direction);
	private:
		WorldViewConfig m_config;
	};
}