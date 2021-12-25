#include "Newton_solver.h"
#include "profiler.h"
#include "devices.h"
#include "raw_pointer_kernels.h"
#include "simulation_data.h"

namespace clumsy_engine
{
	void Newton_Solver::solve()
	{
		RECORD_FUNCTION_DURATION();

		auto device = get_morphism<Device>();


		auto last_pos = m_datas->get_device_pointer<data::Last_Frame_Position>(device);
		auto cur_pos = m_datas->get_device_pointer<data::Position>(device);
		//auto aa = lastPos.ptr;
		float a[1];
		float b[]{2.f};

		device->loop(Copy{ last_pos.ptr,cur_pos.ptr }, cur_pos.size);
		std::vector<vec3f> h_pos;
		device->download(h_pos, last_pos);
		printf("last pos is %f \n", h_pos[1](1));
	}
}
