#include "scene.h"

#include "scene_title.h"

namespace hoge
{
	void SceneManager::Initialize()
	{
		// set empty scene
		m_scene_current = std::make_unique<Scene>();
		m_scene_current->Initialize();
	}
	
	void SceneManager::Finalize()
	{
		m_scene_current->Finalize();
	}

	void SceneManager::Update()
	{
		UpdateTransfer();
		m_scene_current->Update();
	}

	void SceneManager::Draw()
	{
		m_scene_current->Draw();
	}

	void SceneManager::SetNextScene(SceneName next)
	{
		m_scene_name_next = next;
	}
	void SceneManager::UpdateTransfer()
	{
		if (m_scene_name_next == SceneName::SCENE_NONE)
		{
			return;
		}
		// finalize current scene
		m_scene_current->Finalize();
		// initialize next scene
		switch (m_scene_name_next)
		{
		case SceneName::SCENE_TITLE:
		{
			m_scene_current = std::make_unique<SceneTitle>();
			break;
		}
		case SceneName::SCENE_GAME:
		{
			break;
		}
		case SceneName::SCENE_ENDING:
		{
			break;
		}
		}
		m_scene_current->Initialize();
		m_scene_name_current = m_scene_name_next;
		m_scene_name_next = SceneName::SCENE_NONE;
	}
}

