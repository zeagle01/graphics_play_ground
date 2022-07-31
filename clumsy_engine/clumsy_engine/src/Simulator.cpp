



#include "simulator.h"
#include "interaction.h"
#include "inertial.h"
#include "spring_stretch.h"
#include "profiler.h"

#include "gravity.h"
#include "Simulation_Data.h"
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
		m_data_map = build_dependent_variable_set<clumsy_lib::Variable_Set, data>();

		m_data_accecor.set_type_map(m_data_map);

		m_morphisms.add_types<Morphism>();

		using Morphism_List = clumsy_lib::extract_member_type_list_t<Morphism>;
		//printf("%s\n", typeid(Morphism).name());
		clumsy_lib::For_Each_Type<Morphism_List>::template apply< Add_Morphism_Types>(m_morphisms);
	}


	void Simulator::update()
	{
		RECORD_FUNCTION_DURATION();

		auto& simulation_solver = get_morphism<Simulation_Solver>();

		simulation_solver->set_data_ref(&m_data_accecor);
		simulation_solver->set_interactions(&m_interactions_map);
		simulation_solver->set_morphism_ref(&m_morphisms);

		simulation_solver->solve();

	}

}