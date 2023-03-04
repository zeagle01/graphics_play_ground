
#include "system_equations.h"
#include "clumsy_lib/type_map.h"
#include "dependent_variable_set.h"
#include "clumsy_lib/morphism_types.h"


namespace clumsy_engine
{
	class Interaction;
	class Simulation_Solver_CPU_Serial
	{
	public: 
		void solve();
		void set_data_ref(clumsy_lib::Variable_Accecor<clumsy_lib::Type_Map<void>>* datas)
		{
			m_datas = datas;
		}

		void set_interactions(clumsy_lib::Type_Map<Interaction>* interactions)
		{
			m_interactions = interactions;
		}

		void set_morphism_ref(clumsy_lib::Type_Map<>* morphisms)  
		{
			m_morphisms = morphisms;
		}

	private:

		void assemble_equations();

		template<typename Morphism_T>
		auto& get_morphism()
		{
			return *(m_morphisms->get_type<clumsy_lib::Morphism_Types<typename Morphism_T::base_type>>());
		}

		clumsy_lib::Variable_Accecor<clumsy_lib::Type_Map<void>>* m_datas;
		clumsy_lib::Type_Map<Interaction>* m_interactions;
		clumsy_lib::Type_Map<>* m_morphisms;

		std::vector<Element_Equation> m_equations;
	};

}
