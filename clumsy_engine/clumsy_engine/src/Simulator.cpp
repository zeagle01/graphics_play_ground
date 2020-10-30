



#include "simulator.h"
#include "interaction.h"
#include "system_equations_solver.h"
#include "inertial.h"

#include "gravity.h"
#include "Simulation_Data.h"


namespace clumsy_engine
{

	Simulator::Simulator()
	{
		m_sim_data = std::make_shared <Simulation_Data>();
	}

	void Simulator::set_mesh(std::vector<float> positions, std::vector<int> triangles)
	{
		m_sim_data->set_mesh(positions, triangles);
	}

	void Simulator::add_interaction(std::unique_ptr<Interaction> interaction)
	{
		m_interations.push_back(std::move(interaction));
	}

	std::vector<float> Simulator::get_delta_positions()
	{
		std::vector<float> ret(m_sim_data->m_positions.size());

		for (int i = 0; i < m_sim_data->m_positions.size(); i++)
		{
			ret[i] = m_sim_data->m_positions[i] - m_sim_data->m_last_frame_positions[i];
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


		//std::vector<Constraint> constraints;

		std::vector<Element_Equation> equations;
		for (auto& interation : m_interations)
		{
			auto ieraction_equations = interation->compute_element_equations(m_sim_data);
			equations.insert(equations.end(), ieraction_equations.begin(), ieraction_equations.end());
		}


		///////////// update////////////
		m_sim_data->m_last_frame_positions = m_sim_data->m_positions;

		m_solver->solve(m_sim_data->m_positions, equations);

		auto inertial = m_interactions_map.get<Inertial>();

		if (inertial)
		{
			float time_step = inertial->get_time_step();

			for (int i = 0; i < m_sim_data->m_positions.size(); i++)
			{
				m_sim_data->m_velocities[i] = (m_sim_data->m_positions[i] - m_sim_data->m_last_frame_positions[i]) / time_step;
			}
		}
	}

}