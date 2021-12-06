#pragma once

namespace engine {

	class Time {
	public:
		Time(float time = 0.0f) : m_Time(time) {}
		// Integrate float as a viable operator
		operator float() const { return m_Time; }

		float getSeconds() const { return m_Time; }
		float getMilliseconds() const { return m_Time * 1000.0f; }
	
	private:
		float m_Time;
	};

}