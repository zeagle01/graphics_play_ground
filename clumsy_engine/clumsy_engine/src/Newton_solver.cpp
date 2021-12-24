#include "Newton_solver.h"
#include "profiler.h"
#include "devices.h"
#include "raw_pointer_kernels.h"

namespace clumsy_engine
{
	void Newton_Solver::solve()
	{
		RECORD_FUNCTION_DURATION();

		Device_Base device{ CPU_Serial{} };

		float a[1];
		float b[]{2.f};

		device.loop(Copy{a,b}, 1);

	}
}
