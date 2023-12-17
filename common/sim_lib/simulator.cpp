

module;

module sim_lib;


namespace sim_lib
{

	bool simulator::step()
	{
		return m_imp.step();
	}

	void simulator::init()
	{
		m_imp.init();
	}

}