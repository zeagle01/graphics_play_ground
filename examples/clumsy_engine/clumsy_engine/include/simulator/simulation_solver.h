
#include "clumsy_lib/class_reflection.h"
#include "simulation_solver_cpu_serial.h"
#include "Newton_solver.h"

namespace clumsy_engine
{
	class Simulation_Solver_Base
	{
	public:
		virtual void solve() = 0;
		virtual void set_data_ref(clumsy_lib::Variable_Accecor<clumsy_lib::Type_Map<void>>* datas) = 0;
		virtual void set_interactions(clumsy_lib::Type_Map<Interaction>* interactions) = 0;
		virtual void set_morphism_ref(clumsy_lib::Type_Map<>* morphisms) = 0;
	};


	template<typename Imp_T>
	class Simulation_Solver_Wrapper :public Simulation_Solver_Base
	{
	public:

		void solve() override
		{
			imp.solve();
		}

		void set_data_ref(clumsy_lib::Variable_Accecor<clumsy_lib::Type_Map<void>>* datas) override
		{
			imp.set_data_ref(datas);
		}

		void set_interactions(clumsy_lib::Type_Map<Interaction>* interactions) override
		{
			imp.set_interactions(interactions);

		}

		void set_morphism_ref(clumsy_lib::Type_Map<>* morphisms)  override
		{
			imp.set_morphism_ref(morphisms);
		}

		Imp_T imp;
	};

#define ADD_SIMULATION_SOLVER(name,solver_imp) ADD_TYPE_TO_GROUP(name, Simulation_Solver_Wrapper, solver_imp);

	struct Simulation_Solver
	{
		using base_type = Simulation_Solver_Base;

		ADD_SIMULATION_SOLVER(CPU_Serial, Simulation_Solver_CPU_Serial);
		ADD_SIMULATION_SOLVER(Newton, Newton_Solver);
	};


}