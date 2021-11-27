
#include "simulation_solver_cpu_serial.h"

namespace clumsy_engine
{

	void Simulation_Solver_CPU_Serial::solve() 
	{
//		//back up last
//		auto positions = get_value<data::Position>();
//		set_value<data::Last_Frame_Position>(positions);

//		assemble_equations();
//
//		auto& linear_solver = get_morphism<Linear_Equations_Solver>();
//		positions = linear_solver->solve(positions, m_equations);
//		set_value<data::Position>(positions);


//		auto inertial = m_interactions_map.get_type<interaction::Inertial>();
//
//		if (inertial)
//		{
//			float time_step = get_value<data::Time_Step>();
//
//			std::vector<vec3f> velocity(positions.size());
//
//			for (int i = 0; i < positions.size(); i++)
//			{
//				velocity[i] = (positions[i] - get_value<data::Last_Frame_Position>()[i]) / time_step;
//			}
//
//			set_value<data::Velocity>(velocity);//maybe optimization?
//		}

	}
}
