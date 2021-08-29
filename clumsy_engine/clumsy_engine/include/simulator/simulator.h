

#pragma once



#include <vector>
#include <memory>
#include <map>
#include <string>

#include "clumsy_lib/type_map.h"
#include "clumsy_lib/morphism_types.h"
#include "clumsy_lib/dependent_variable_set.h"
#include "Simulation_Data.h"
#include "system_equations.h"
#include "linear_equations_solver.h"



namespace clumsy_engine
{

	class Interaction;


	class Simulator :public clumsy_lib::Variable_Accecor<clumsy_lib::Type_Map<void>>
	{
	public:

		Simulator();

		void update();


		template<typename Inter,typename ...P>
		void add_interaction(P&& ... p)
		{
			auto inter = std::make_shared<Inter>(std::forward<P>(p)...);

			m_interactions_map.add_type(inter);

			inter->set_variable_set(m_data_map);

		}

		template<typename Inter>
		void remove_interaction()
		{
			m_interactions_map.remove_type<Inter>();
		}



		template<typename Morphism_T,typename Sub_T>
		void set_morphism_type()
		{
			auto m = m_morphisms.get_type<clumsy_lib::Morphism_Types<Morphism_T::base_type>>();
			m->set_current_type<Sub_T>();
		}


		


	private:

		template<typename Morphism_T>
		auto& get_morphism() 
		{
			return *(m_morphisms.get_type<clumsy_lib::Morphism_Types<Morphism_T::base_type>>());
		}


		void assemble_equations();

		clumsy_lib::Type_Map<Interaction> m_interactions_map;
		std::shared_ptr<clumsy_lib::Variable_Set> m_data_map;
		clumsy_lib::Type_Map<> m_morphisms;

		std::vector<Element_Equation> m_equations;

	};

}



