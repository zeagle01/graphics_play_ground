




#include "profiler.h"
#include <algorithm>
#include <string>
#include <thread>
#include "log.h"


namespace clumsy_engine
{



		Scope_Timer::Scope_Timer(std::string name) 
			:
			m_name(name),
			m_begin(std::chrono::steady_clock::now())
		{
		};

		Scope_Timer::~Scope_Timer()
		{
			std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
			long begin_t = std::chrono::time_point_cast<std::chrono::microseconds>(m_begin).time_since_epoch().count();
			long end_t = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count();
			uint32_t thread_id = std::hash<std::thread::id>{} (std::this_thread::get_id());

			Profiler::get_singleton().write_duration_record({ begin_t,end_t ,m_name,thread_id});
		}


		void Profiler::write_duration_record(const Duration& duration)
		{


			std::string name = duration.name;
			std::replace(name.begin(), name.end(), '"', '\'');

			if (m_record_count++ > 0)
			{
				m_os << ",";
			}

			m_os << "{";
			m_os << "\"cat\":\"function\",";
			m_os << "\"dur\":" << duration.end - duration.begin << ",";
			m_os << "\"name\":\"" << name << "\",";
			m_os << "\"ph\":\"X\",";
			m_os << "\"pid\":0,";
			m_os << "\"tid\":" << duration.thread_id << ",";
			m_os << "\"ts\":" << duration.begin;
			m_os << "}";

			m_os.flush();
		}

		Profiler& Profiler::get_singleton()
		{
			static std::unique_ptr<Profiler> singleton = std::make_unique<Profiler>();
			return *singleton;
		}


		void Profiler::begin_session(std::string session_name,std::string output_file)
		{
			m_os.open(output_file);
			if (m_os.good())
			{
				m_os << "{\"otherData\":{},\"traceEvents\":[";
				m_os.flush();
			}
			else
			{
				CE_CORE_ERROR("can't open {}", output_file);
			}
		}

		void Profiler::end_session()
		{
			m_os << "]}";
			m_os.flush();
			m_os.close();

			m_record_count = 0;
		}


}
