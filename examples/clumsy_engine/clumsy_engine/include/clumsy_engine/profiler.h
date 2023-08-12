


#pragma once

#include <chrono>
#include <string>
#include <fstream>




#define PROFILING

#ifdef PROFILING
#define FUNCTION_SCOPE_TIMER(function_name) clumsy_engine::Scope_Timer time##__LINE__(function_name)

#define RECORD_FUNCTION_DURATION() FUNCTION_SCOPE_TIMER(__FUNCSIG__)
#define BEGIN_PROFILING() clumsy_engine::Profiler::get_singleton().begin_session(__FUNCSIG__)
#define END_PROFILING() clumsy_engine::Profiler::get_singleton().end_session()

#else
#define RECORD_FUNCTION_DURATION() 
#define BEGIN_PROFILING()
#define END_PROFILING()

#endif


namespace clumsy_engine
{



	struct Duration
	{
		long begin;
		long end;
		std::string name;
		uint32_t thread_id;
	};


	class Scope_Timer
	{

	public:
		Scope_Timer(std::string name);
		~Scope_Timer();

	private:
		std::chrono::time_point<std::chrono::steady_clock> m_begin;
		std::string m_name;
	};



	class Profiler
	{
	public:
		Profiler() {}

		void begin_session(std::string session_name,std::string output_file="time_line.json");

		void end_session();

		void write_duration_record(const Duration& duration);

		static Profiler& get_singleton();

	private:
		std::ofstream m_os;
		int m_record_count = 0;
	};


}

