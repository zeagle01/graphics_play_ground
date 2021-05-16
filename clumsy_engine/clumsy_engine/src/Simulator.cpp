



#include "simulator.h"
#include "interaction.h"
#include "inertial.h"
#include "spring_stretch.h"
#include "profiler.h"

#include "gravity.h"
#include "Simulation_Data.h"
#include "clumsy_lib/type_list.h"


namespace clumsy_engine
{
	//using namespace data;

	Simulator::Simulator()
	{

		m_data_map = std::make_shared<Simulation_Datas>();

		set_(m_data_map);

		clumsy_lib::for_each_type<all_types, build_sim_data>(*m_data_map);

		for_each_depend_type<all_types ,build_dependent>(*m_data_map);

	}

	void Simulator::assemble_equations()
	{
		RECORD_FUNCTION_DURATION();

		m_equations.resize(0);

		for (auto& it : m_interactions_map)
		{
			auto& interation = it.second;
			auto ieraction_equations = interation->compute_element_equations();
			m_equations.insert(m_equations.end(), ieraction_equations.begin(), ieraction_equations.end());
		}

	}

	void Simulator::update()
	{
		RECORD_FUNCTION_DURATION();

		std::unique_ptr<System_Equations_Solver> m_solver;
		if (!m_solver)
		{
			m_solver = std::make_unique<System_Equations_Solver>();
		}

		//back up last
		auto positions = get<data::Position>();
		set<data::Last_Frame_Position>(positions);


		///////////// update////////////
		for (int i = 0; i < 1; i++)
		{
			assemble_equations();
			m_solver->solve(positions, m_equations);
		}
		set<data::Position>(positions);


		auto inertial = m_interactions_map.get<Inertial>();

		if (inertial)
		{
			float time_step = get<data::Time_Step>();

			std::vector<vec3f> velocity(positions.size());

			for (int i = 0; i < positions.size(); i++)
			{
				velocity[i] = (positions[i] - get<data::Last_Frame_Position>()[i]) / time_step;
			}

			set<data::Velocity>(velocity);//maybe optimization?
		}
	}

}