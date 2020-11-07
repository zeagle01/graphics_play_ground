

#pragma once

#include "system_equations_solver.h" 
#include "Simulation_Data.h"
#include <memory>
#include <functional>

namespace clumsy_engine
{

	class Simulator;

	using stencil = std::vector<int>;

	class Interaction
	{
	public:
		virtual ~Interaction() {}

		virtual std::vector<stencil> compute_stencils() { return std::vector<stencil>(); }

		virtual Element_Equation compute_element_equation(stencil st) { return Element_Equation(); }

		std::vector<Element_Equation> compute_element_equations();




//
//
//		template<typename tl>
//		void set_up_data(Simulation_Datas& all_sim_data)
//		{
//			for_each_type<tl, extract_sim_data>(m_sim_datas, all_sim_data);
//		}
//
//		template<typename tl>
//		void set_up_data()
//		{
//			build_sim_datas<tl>::apply(m_sim_datas);
//		}

//	protected:
//		template<typename T>
//		const auto& get_data()
//		{
//			return m_sim_datas.get_data<T>();
//		}
//
//	private:
//		Simulation_Datas m_sim_datas;
	};

}
