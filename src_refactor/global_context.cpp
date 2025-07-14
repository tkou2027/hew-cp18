#include "global_context.h"

// platform
#include "window.h"
#include "buffer.h"
#include "timer.h"
#include "audio.h"
// game
#include "world.h"
#include "scene.h"

namespace hoge
{
	GlobalContext g_global_context;

	void GlobalContext::StartSystems()
	{
		m_window = std::make_shared<Window>();
		WindowCreateInfo window_create_info;
		m_window->Initialize(window_create_info);

		m_buffer = std::make_shared<Buffer>();
		m_buffer->Initialize(80, 25);

		m_timer = std::make_shared<Timer>();
		m_timer->Initialize();

		m_audio = std::make_shared<Audio>();
		m_audio->Initialize();

		m_world = std::make_shared<World>();
		m_world->Initialize();

		m_scene_manager = std::make_shared<SceneManager>();
		m_scene_manager->Initialize();
	}

	void GlobalContext::ShutdownSystems()
	{
		m_scene_manager->Finalize();
		m_world->Finalize();

		m_audio->Finalize();
		m_timer->Initialize();
		
		m_buffer->Finalize();
		m_window->Finalize();
	}

}