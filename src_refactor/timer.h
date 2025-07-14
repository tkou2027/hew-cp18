#pragma once

#include <chrono>

namespace hoge
{
	struct Rhythm {
		float bpm{ 0.0f };
		float loop_length{ 0.0f };
		float step_factor{ 0.0f };
	};

	class Timer
	{
	public:
		void Initialize();
		void Finalize();
		void Update();
		float GetTimeSinceLastFrame();

		void SetRhythm(const Rhythm& rhythm);
		float GetRhythmStep();
		float GetRhythmPosition();
	private:
		std::chrono::steady_clock::time_point m_last_time_point{ std::chrono::steady_clock::now() };

		int m_delta_time_milli{ 0 };
		float m_delta_time{ 0.0f }; // seconds

		// rhythm
		Rhythm m_rhythm;
		// calculated
		float m_rhythm_step{ 1.0f }; // minimal step
		float m_rhythm_step_inv{ 1 };
		// int rhythm_start_time_milli{ 0 }; // milliseconds
		std::chrono::steady_clock::time_point m_rhythm_start_time_point;
		float m_rhythm_position{ 0.0f };
	};
}