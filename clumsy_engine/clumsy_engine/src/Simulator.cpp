



#include "simulator.h"
#include "interaction.h"
#include "system_equations_solver.h"
#include "inertial.h"


namespace clumsy_engine
{
	void Simulator::add_interaction(std::unique_ptr<Interaction> interaction)
	{
		m_interations.push_back(std::move(interaction));
	}

	std::vector<float> Simulator::get_delta_positions()
	{
		if (m_positions0.empty())
		{
			m_positions0.resize(m_positions.size());
			return m_positions0;
		}

		std::vector<float> ret(m_positions.size());
		for (int i = 0; i < m_positions.size(); i++)
		{
			ret[i] = m_positions[i] - m_positions0[i];
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
		if (m_velocity.empty())
		{
			m_velocity.resize(m_positions.size());
		}
		if (m_positions0.empty())
		{
			m_positions0 = m_positions;
		}

		int vertex_num = m_positions.size() / 3;

		std::vector<float> masses(vertex_num, 1.f);

		add_interaction(std::make_unique<Inertial>(vertex_num, masses, m_positions0, m_velocity, m_time_step));

		std::vector<Element_Equation> equations;

		for (auto& interation : m_interations)
		{
			auto ieraction_equations = interation->compute_equations();

			equations.insert(equations.end(), ieraction_equations.begin(), ieraction_equations.end());
		}






		///////////// update////////////
		m_positions0 = m_positions;

		m_solver->solve(m_positions, equations);

		for (int i = 0; i < m_positions.size(); i++)
		{
			m_velocity[i] = (m_positions[i] - m_positions0[i]) / m_time_step;
		}
	}

}