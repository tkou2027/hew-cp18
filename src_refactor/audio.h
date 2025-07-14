#pragma once

#include <cstddef>
#include <array>
#include <string>
#include "timer.h"

namespace hoge
{
	class Audio
	{
	public:
		enum class AudioBgm {
			// begin
			GAME,
			TITLE,
			// end
			NUM, // number of BGMs
			NONE // nothing playing
		};
		enum class AudioSe {
			// begin
			ITEM_GAIN,
			ITEM_GAIN_BAD,
			ITEM_SPAWN,
			ITEM_SPOT,
			SELECT,
			JUMP,
			FALL,
			// end
			NUM // number of SEs
		};
		void Initialize();
		void Finalize();
		void PlayerBgm(AudioBgm bgn);
		void PlaySoundEffect(AudioSe sound);
	private:
		std::array<Rhythm, static_cast<size_t>(AudioBgm::NUM)> m_bgm_infos{};
		AudioBgm m_bgm_playing{ AudioBgm::NONE };
		// std::array<bool, static_cast<std::size_t>(AudioSe::NUM)> m_se_playing{};

		std::wstring GetAliasBgm(AudioBgm bgm);
		std::wstring GetAliasSe(AudioSe se);
		void OpenAudio(const std::wstring& alias, const std::wstring& filename);
		void PlayAudio(const std::wstring& alias, bool loop = false);
		void StopAudio(const std::wstring& alias);
		void CloseAudio(const std::wstring& alias);
	};
}