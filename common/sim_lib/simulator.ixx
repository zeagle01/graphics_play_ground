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
		void set(typename const var::type& data)
		{
			m_imp.set<var>(data);
		}

		template<typename var>
		const typename var::type& get()
		{
			return m_imp.get<var>();
		}

		void init();
		void step();


	private:
		simulator_imp m_imp;
	};

}
