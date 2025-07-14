#include "application.h"
#include "global_context.h"
#include "buffer.h"
#include "timer.h"
#include "scene.h"


namespace hoge
{
	void Application::Initialize(const wchar_t* title)
	{
		g_global_context.StartSystems();

		g_global_context.m_scene_manager->SetNextScene(SceneManager::SceneName::SCENE_TITLE);
	}

	void Application::Run()
	{
		while (true)
		{
			float delta_time = g_global_context.m_timer->GetTimeSinceLastFrame();
			if (delta_time >= 1.0f / 60.0f) // 60fps
			{
				Update();
				Draw();
			}
		}
	}

	void Application::Finalize()
	{
		g_global_context.ShutdownSystems();
	}

	void Application::Update()
	{
		g_global_context.m_timer->Update();

		g_global_context.m_scene_manager->Update();
	}

	void Application::Draw()
	{
		g_global_context.m_scene_manager->Draw();

		g_global_context.m_buffer->PresentSwap();
	}
}