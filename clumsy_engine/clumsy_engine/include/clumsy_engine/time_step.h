
#pragma once


namespace clumsy_engine
{


	class Time_Step
	{
		public:
			Time_Step(float dt) :m_time(dt) {};

			operator float() const { return m_time; }

			float get_seconds() const { return m_time; };
			float get_milliseconds() const { return m_time*1000.f; };

	private:
		float m_time = 0.f;
	};
}

