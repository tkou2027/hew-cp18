#pragma once

#include "scene.h"
#include "sprite.h"

namespace hoge
{
	class SceneTitle : public Scene
	{
	public:
		void Initialize() override;
		void Update() override;
		void Draw() override;
	private:
		std::unique_ptr<Sprite> m_title_text;
	};
}