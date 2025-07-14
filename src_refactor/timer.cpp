#include "timer.h"

namespace hoge
{
	void Timer::Initialize()
	{
		m_last_time_point = std::chrono::steady_clock::now();
	}

	void Timer::Finalize() {}

	void Timer::Update()
	{
		{
			using namespace std::chrono;

			steady_clock::time_point now_time_point = steady_clock::now();
			duration<float> time_span = duration_cast<duration<float>>(now_time_point - m_last_time_point);
			// update delta time
			m_delta_time = time_span.count();
			// update last tick time
			m_last_time_point = now_time_point;
		}
	}

	float Timer::GetTimeSinceLastFrame()
	{
		{
			using namespace std::chrono;

			steady_clock::time_point now_time_point = steady_clock::now();
			duration<float> time_span = duration_cast<duration<float>>(now_time_point - m_last_time_point);
			return time_span.count();
		}
	}

	void Timer::SetRhythm(const Rhythm& rhythm)
	{
		m_rhythm = rhythm;
		float crotchet = 60.0f / rhythm.bpm;
		m_rhythm_step = crotchet * rhythm.step_factor; // beats per crotchet
		// avoid divide zero
		m_rhythm_step_inv = m_rhythm_step > 1e-4 ? 1.0f / m_rhythm_step : 1.0f;

		// rhythm timer
		m_rhythm_start_time_point = m_last_time_point; // TODO: check if this is the case
		m_rhythm_position = 0.0f;
	}

	float Timer::GetRhythmStep()
	{
		return m_rhythm_step;
	}

	float Timer::GetRhythmPosition()
	{
		return m_rhythm_position;
	}

}
