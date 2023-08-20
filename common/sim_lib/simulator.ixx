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
		template<typename T>
		void set_data()
		{
			m_imp.set_data<T>();
		}

		void start();
		void end();

		void step();

	private:
		simulator_imp m_imp;
	};

}
