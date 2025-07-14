#include "audio.h"

// mciSendString
#include <sstream>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// timer
#include "global_context.h"

namespace hoge
{
	void Audio::Initialize()
	{
		// load all audios here (as there aren't many)
		// BGMs
		OpenAudio(GetAliasBgm(AudioBgm::GAME), L"assets/audios/MesmerizingGalaxyLoop.mp3");
		m_bgm_infos[static_cast<size_t>(AudioBgm::GAME)] = Rhythm{
			124,// bpm
			92970, // song length
			1
		};
		OpenAudio(GetAliasBgm(AudioBgm::TITLE), L"assets/audios/GalacticRap.mp3");
		m_bgm_infos[static_cast<size_t>(AudioBgm::TITLE)] = Rhythm {
			120,// bpm
			142054, // song length
			1
		};

		OpenAudio(GetAliasSe(AudioSe::ITEM_GAIN),     L"assets/audios/se_itemget_009.mp3");
		OpenAudio(GetAliasSe(AudioSe::ITEM_GAIN_BAD), L"assets/audios/maou_se_system39.mp3");
		OpenAudio(GetAliasSe(AudioSe::ITEM_SPAWN),    L"assets/audios/se_powerup_006.mp3");
		OpenAudio(GetAliasSe(AudioSe::ITEM_SPOT),     L"assets/audios/maou_se_system46.mp3");
		OpenAudio(GetAliasSe(AudioSe::SELECT),        L"assets/audios/maou_se_system41.mp3");
		OpenAudio(GetAliasSe(AudioSe::JUMP),          L"assets/audios/se_jump1.mp3");
		OpenAudio(GetAliasSe(AudioSe::FALL),          L"assets/audios/se_select_006.mp3");
	}

	void Audio::Finalize()
	{
		for (int i = 0; i < static_cast<int>(AudioBgm::NUM); ++i)
		{
			CloseAudio(GetAliasBgm(static_cast<AudioBgm>(i)));
		}
		for (int i = 0; i < static_cast<int>(AudioSe::NUM); ++i)
		{
			CloseAudio(GetAliasSe(static_cast<AudioSe>(i)));
		}
	}

	void Audio::PlayerBgm(AudioBgm bgm)
	{
		// one bgm at a time, stop current playing bgm
		if (m_bgm_playing != AudioBgm::NONE)
		{
			StopAudio(GetAliasBgm(m_bgm_playing));
		}
		// play loop
		PlayAudio(GetAliasBgm(bgm),true);
		// set rhythm with predefined song info, even if the file does not exist
		g_global_context.m_timer->SetRhythm(m_bgm_infos[static_cast<size_t>(bgm)]);
		m_bgm_playing = bgm;
	}

	void Audio::PlaySoundEffect(AudioSe se)
	{
		PlayAudio(GetAliasSe(se));
	}

	// Play MP3 Audios using mciSendString ========
	std::wstring Audio::GetAliasBgm(AudioBgm bgm)
	{
		std::wstringstream ss;
		ss << L"bgm_" << static_cast<int>(bgm);
		return ss.str();
	}
	std::wstring Audio::GetAliasSe(AudioSe se)
	{
		std::wstringstream ss;
		ss << L"se_" << static_cast<int>(se);
		return ss.str();
	}
	void Audio::OpenAudio(const std::wstring& alias, const std::wstring& filename)
	{
		std::wstringstream cmd;
		cmd << "open \"" << filename << "\" type mpegvideo alias " << alias;
		mciSendString(cmd.str().c_str(), nullptr, 0, nullptr);
	}
	void Audio::PlayAudio(const std::wstring& alias, bool loop)
	{
		std::wstringstream cmd;
		cmd << "play " << alias;
		if (loop)
		{
			cmd << " repeat";
		}
		mciSendString(cmd.str().c_str(), nullptr, 0, nullptr);
	}
	void Audio::StopAudio(const std::wstring& alias)
	{
		std::wstringstream cmd;
		cmd << "stop " << alias;
		mciSendString(cmd.str().c_str(), nullptr, 0, nullptr);
	}
	void Audio::CloseAudio(const std::wstring& alias)
	{
		std::wstringstream cmd;
		cmd << "close " << alias;
		mciSendString(cmd.str().c_str(), nullptr, 0, nullptr);
	}
	// Play MP3 Audios using mciSendString ========
}
