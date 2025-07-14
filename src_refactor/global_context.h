#pragma once

#include <memory>

namespace hoge
{
	class Window;
	class Buffer;
	class Timer;
	class Audio;
	class World;
	class SceneManager;


	class GlobalContext
	{
	public:
		void StartSystems();
		void ShutdownSystems();

		std::shared_ptr<Window> m_window;
		std::shared_ptr<Buffer> m_buffer;
		std::shared_ptr<Timer> m_timer;
		std::shared_ptr<Audio> m_audio;
		std::shared_ptr<World> m_world;
		std::shared_ptr<SceneManager> m_scene_manager;
	};

	extern GlobalContext g_global_context;
}