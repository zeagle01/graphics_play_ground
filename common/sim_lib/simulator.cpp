

module;

module sim_lib;


namespace sim_lib
{

	void simulator::step()
	{
		m_imp.step();
	}

	void simulator::init()
	{
		m_imp.init();
	}

}