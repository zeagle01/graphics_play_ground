
#include "clumsy_lib/class_reflection.h"
#include "simulation_solver_cpu_serial.h"

namespace clumsy_engine
{


	class Simulation_Solver_Base
	{
	public:
		virtual void solve() = 0;
	};


	template<typename Imp_T>
	class Simulation_Solver_Wrapper :public Simulation_Solver_Base
	{
	public:

		void solve() override
		{
			imp.solve();
		}

		Imp_T imp;
	};

#define ADD_SIMULATION_SOLVER(name,solver_imp) ADD_TYPE_TO_GROUP(name, Simulation_Solver_Wrapper, solver_imp);

	struct Simulation_Solver
	{
		using base_type = Simulation_Solver_Base;

		ADD_SIMULATION_SOLVER(CPU_Serial, Simulation_Solver_CPU_Serial);
	};


}