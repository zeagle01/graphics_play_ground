



#include "simulator.h"
#include "interaction.h"
#include "system_equations_solver.h"
#include "inertial.h"
#include "spring_stretch.h"

#include "gravity.h"
#include "Simulation_Data.h"


namespace clumsy_engine
{
	//using namespace data;

	Simulator::Simulator()
	{

		for_each_type<all_types, build_sim_data>(m_data_map);
		for_each_depend_type<all_types ,build_dependent>(m_data_map);

	}




	std::vector<float> Simulator::get_delta_positions()
	{
		const auto& positions = m_data_map.get_data<data::Position>();

		std::vector<float> ret(positions.size());

		for (int i = 0; i < positions.size(); i++)
		{
			ret[i] = positions[i] - get<data::Last_Frame_Position>()[i];
		}
		return ret;
	}

	void Simulator::update()
	{

		std::unique_ptr<System_Equations_Solver> m_solver;
		if (!m_solver)
		{
			m_solver = std::make_unique<System_Equations_Solver>();
		}

		std::vector<Element_Equation> equations;
		for (auto& it : m_interactions_map)
		{
			auto& interation = it.second;
			auto ieraction_equations = interation->compute_element_equations();
			equations.insert(equations.end(), ieraction_equations.begin(), ieraction_equations.end());
		}


		///////////// update////////////
		const auto& m_positions = get<data::Position>();
		auto positions = m_positions;
		set<data::Last_Frame_Position>(positions);

		m_solver->solve(positions, equations);

		set<data::Position>(positions);

		auto inertial = m_interactions_map.get<Inertial>();

		if (inertial)
		{
			float time_step = get<data::Time_Step>();

			std::vector<float> velocity(positions.size());

			for (int i = 0; i < positions.size(); i++)
			{
				velocity[i] = (positions[i] - get<data::Last_Frame_Position>()[i]) / time_step;
			}

			set<data::Velocity>(velocity);//maybe optimization?
		}
	}

	std::vector<float> Simulator::get_edge_lengths()
	{
		return { 1. };
	}

}