module;

//include things here

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

		[[nodiscard]] bool commit_all_changes()
		{
			return m_imp.commit_all_changes();
		}

		void init();
		void step();


	private:
		simulator_imp m_imp;
	};

}
