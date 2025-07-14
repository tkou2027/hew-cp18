#pragma once

#include <memory>

namespace hoge
{
	class Scene
	{
	public:
		virtual ~Scene() = default;
		virtual void Initialize() {};
		virtual void Update() {};
		virtual void Draw() {};
		virtual void Finalize() {};
	};

	class SceneManager
	{
	public:
		enum class SceneName
		{
			SCENE_NONE,
			SCENE_TITLE,
			SCENE_GAME,
			SCENE_ENDING
		};
		void Initialize();
		void Finalize();
		void Update();
		void Draw();
		void SetNextScene(SceneName next);
	private:
		void UpdateTransfer();

		SceneName m_scene_name_current{ SceneName::SCENE_TITLE };
		SceneName m_scene_name_next{ SceneName::SCENE_NONE };
		std::unique_ptr<Scene> m_scene_current;
	};
}