#include "Newton_solver.h"
#include "profiler.h"
#include "devices.h"
#include "raw_pointer_kernels.h"
#include "simulation_data.h"
#include "interaction.h"
#include "linear_equations_solver.h"

namespace clumsy_engine
{



	//TODO:
	struct Sparse_Matrix_CSR
	{
		void assemble(std::vector<Element_Equation_Pointer>& element_equation_pointers)
		{
			//compute pattern
			for (const auto& eq : element_equation_pointers)
			{

			}

			// assemble
			for (const auto& eq : element_equation_pointers)
			{

			}

		}
	};




	void Newton_Solver::solve()
	{
		RECORD_FUNCTION_DURATION();

		auto device = get_morphism<Device>();


		auto last_pos = m_datas->get_device_pointer<data::Last_Frame_Position>(device);
		auto cur_pos = m_datas->get_device_pointer<data::Position>(device);
		//auto aa = lastPos.ptr;
		float a[1];
		float b[]{ 2.f };

		std::vector<Element_Equation_Pointer> element_equation_pointers;
		for (auto& interation : *m_interactions)
		{
			auto element_equation_pointer = interation->compute_element_equations_pointers();
			element_equation_pointers.push_back(element_equation_pointer);
		}

		auto& linear_solver = get_morphism<Linear_Equations_Solver>();
		linear_solver->solve(cur_pos.ptr, element_equation_pointers, cur_pos.size);

		device->loop(Copy{ last_pos.ptr,cur_pos.ptr }, cur_pos.size);
		std::vector<vec3f> h_pos;
		device->download(h_pos, last_pos);
		printf("last pos is %f \n", h_pos[1](1));
	}
}
