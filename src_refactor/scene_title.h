#pragma once

#include "scene.h"
#include "sprite.h"

namespace hoge
{
	class SceneTitle : public Scene
	{
	public:
		void Initialize();
		void Update();
		void Draw();
	private:
		std::unique_ptr<Sprite> m_title_text;
	};
}