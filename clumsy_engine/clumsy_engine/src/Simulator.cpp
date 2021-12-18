



#include "simulator.h"
#include "interaction.h"
#include "inertial.h"
#include "spring_stretch.h"
#include "profiler.h"

#include "gravity.h"
#include "Simulation_Data.h"
#include "simulation_interactions.h"
#include "linear_equations_solver.h"
#include "morphisms.h"

#include "clumsy_lib/type_list.h"
#include "clumsy_lib/static_loop.h"


namespace clumsy_engine
{
	
	struct Add_Morphism_Types
	{
		template<typename T>
		static void apply(auto& morphisms)
		{
			auto m = morphisms.get_type<clumsy_lib::Morphism_Types<T::base_type>>();
			m->add_types<T>();
		}
	};


	Simulator::Simulator()
	{
		m_data_map = clumsy_lib::build_dependent_variable_set<clumsy_lib::Variable_Set, data>();

		m_data_accecor.set_type_map(m_data_map);

		m_morphisms.add_types<Morphism>();

		using Morphism_List = clumsy_lib::extract_member_type_list_t<Morphism>;
		//printf("%s\n", typeid(Morphism).name());
		clumsy_lib::For_Each_Type<Morphism_List>::template apply< Add_Morphism_Types>(m_morphisms);
	}

	void Simulator::assemble_equations()
	{
		RECORD_FUNCTION_DURATION();

		m_equations.resize(0);

		for (auto& interation : m_interactions_map)
		{
			//auto& interation = it.second;
			auto ieraction_equations = interation->compute_element_equations();
			m_equations.insert(m_equations.end(), ieraction_equations.begin(), ieraction_equations.end());
		}

	}

	void Simulator::update()
	{
		RECORD_FUNCTION_DURATION();

		auto& simulation_solver = get_morphism<Simulation_Solver>();

		//TODO: open this
		//simulation_solver->solve();

		//TODO: delete below
		//back up last
		auto positions = get_value<data::Position>();
		set_value<data::Last_Frame_Position>(positions);

		///////////// update////////////

		assemble_equations();

		auto& linear_solver = get_morphism<Linear_Equations_Solver>();
		positions = linear_solver->solve(positions, m_equations);
		set_value<data::Position>(positions);


		auto inertial = m_interactions_map.get_type<interaction::Inertial>();

		if (inertial)
		{
			float time_step = get_value<data::Time_Step>();

			std::vector<vec3f> velocity(positions.size());

			for (int i = 0; i < positions.size(); i++)
			{
				velocity[i] = (positions[i] - get_value<data::Last_Frame_Position>()[i]) / time_step;
			}

			set_value<data::Velocity>(velocity);//maybe optimization?
		}
	}

}