#pragma once

namespace MGE {

	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{
		}

		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }

		float GetFrameRate() const { return m_FrameRate; }
		void SetFrameRate(float frameRate) { m_FrameRate = frameRate; }

	private:
		float m_Time;
		float m_FrameRate;
	};

}