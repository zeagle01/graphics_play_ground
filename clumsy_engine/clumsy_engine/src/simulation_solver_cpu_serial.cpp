
#include "simulation_solver_cpu_serial.h"
#include "simulation_data.h"
#include "profiler.h"
#include "linear_equations_solver.h"
#include "simulation_interactions.h"

namespace clumsy_engine
{


	void Simulation_Solver_CPU_Serial::assemble_equations()
	{
		RECORD_FUNCTION_DURATION();

		m_equations.resize(0);

		for (auto& interation : *m_interactions)
		{
			auto ieraction_equations = interation->compute_element_equations();
			m_equations.insert(m_equations.end(), ieraction_equations.begin(), ieraction_equations.end());
		}

	}

	void Simulation_Solver_CPU_Serial::solve() 
	{
		RECORD_FUNCTION_DURATION();

		//back up last
		auto positions = m_datas->get_value<data::Position>();
		m_datas->set_value<data::Last_Frame_Position>(positions);

		assemble_equations();

		auto& linear_solver = get_morphism<Linear_Equations_Solver>();

		positions = linear_solver->solve(positions, m_equations);
		m_datas->set_value<data::Position>(positions);


		auto inertial = m_interactions->get_type<interaction::Inertial>();

		if (inertial)
		{
			float time_step = m_datas->get_value<data::Time_Step>();

			std::vector<vec3f> velocity(positions.size());
			auto& lastFramePos = m_datas->get_value<data::Last_Frame_Position>();

			for (int i = 0; i < positions.size(); i++)
			{
				velocity[i] = (positions[i] - lastFramePos[i]) / time_step;
			}

			m_datas->set_value<data::Velocity>(velocity);//maybe optimization?
		}

	}
}
