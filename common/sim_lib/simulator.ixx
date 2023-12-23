module;

//#include <span> //can't include this, compile bug?
#include <vector>

export module sim_lib;

export import :sim_data;

import :simulator_imp;

namespace sim_lib
{

	export class simulator
	{
	public:
		template<typename var>
		void set(const auto& data)
		{
			m_imp.set<var>(data);
		}

		template<typename var>
		void set_partially(const std::vector<int>& indices, const auto& data)
		{
			m_imp.set_partially<var>(indices, data);
		}

		template<typename var>
		const typename auto& get() const
		{
			return m_imp.get<var>();
		}

		template<typename var>
		typename auto& get()
		{
			return m_imp.get<var>();
		}

		template<typename var>
		void mark_changed()
		{
			return m_imp.mark_changed<var>();
		}

		void commit_all_changes()
		{
			return m_imp.commit_all_changes();
		}

		void init();

		[[nodiscard]] bool step();


	private:
		simulator_imp m_imp;
	};

}
