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

		auto device = get_morphism<Device>().get_curretn_type();
		auto device_type = get_morphism_type_name<Device>();

		auto last_pos = m_datas->get_device_pointer<data::Last_Frame_Position>(device, device_type);
		auto cur_pos = m_datas->get_device_pointer<data::Position>(device, device_type);

		static float t = 0.f;
		float a = 1e-3f;
		device->loop(DummyMove{ cur_pos.ptr,a,t }, cur_pos.size);
		t += 0.001f;

		auto& h_pos = m_datas->get_ref_value<data::Position>();
		device->download<std::vector<vec3f>>(h_pos, cur_pos);
	}
}
