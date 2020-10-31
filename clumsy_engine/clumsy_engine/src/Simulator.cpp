



#include "simulator.h"
#include "interaction.h"
#include "system_equations_solver.h"
#include "inertial.h"
#include "spring_stretch.h"

#include "gravity.h"
#include "Simulation_Data.h"


namespace clumsy_engine
{

	Simulator::Simulator()
	{
		m_sim_data = std::make_shared <Simulation_Data>();

		add_data<Position>();
		add_data<Last_Frame_Position>();
		add_data<Triangle_Indices>();
	}


	void Simulator::set_mesh(std::vector<float> positions, std::vector<int> triangles)
	{
		set<Position>(positions);
		set<Triangle_Indices>(triangles);

		m_sim_data->set_mesh(positions, triangles);
	}



	std::vector<float> Simulator::get_delta_positions()
	{
		const auto& positions = m_data_map.get<Position>()->get();

		std::vector<float> ret(positions.size());

		for (int i = 0; i < positions.size(); i++)
		{
			ret[i] = positions[i] - get<Last_Frame_Position>()[i];
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
		for (auto& interation : m_interations)
		{
			auto ieraction_equations = interation->compute_element_equations(this);
			equations.insert(equations.end(), ieraction_equations.begin(), ieraction_equations.end());
		}


		///////////// update////////////
		auto& positions = m_data_map.get<Position>()->data;
		set<Last_Frame_Position>(positions);

		m_solver->solve(positions, equations);

		auto inertial = m_interactions_map.get<Inertial>();

		if (inertial)
		{
			float time_step = inertial->get_time_step();

			for (int i = 0; i < positions.size(); i++)
			{
				m_sim_data->m_velocities[i] = (positions[i] - get<Last_Frame_Position>()[i]) / time_step;
			}
		}
	}

	std::vector<float> Simulator::get_edge_lengths()
	{
		return { 1. };
	}

}