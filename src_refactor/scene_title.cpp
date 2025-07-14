#include "scene_title.h"

#include "global_context.h"
#include "buffer.h"
#include "timer.h"
#include "audio.h"
#include "world.h"

namespace hoge
{
	void SceneTitle::Initialize()
	{
		// set world
		g_global_context.m_world->SetConfig(World::WorldConfigName::TITLE);

		// load sprite
		m_title_text = std::make_unique<Sprite>();
		m_title_text->Load(L"assets/title.txt");

		// play bgm
		g_global_context.m_audio->PlayerBgm(Audio::AudioBgm::TITLE);

	}
	void SceneTitle::Update()
	{

	}
	void SceneTitle::Draw()
	{
		m_title_text->Draw(24, 0);

		g_global_context.m_buffer->DrawString(36, 18, L"PRESS ENTER", Color{125, 125, 125}, Color{});
		g_global_context.m_buffer->DrawStringFullwidth(36 + 12, 18, L"ÅZ", Color{ 125, 125, 125 }, Color{});
	}
}

