

#pragma once


namespace clumsy_engine
{

	class Simulation_Datas;

	template<typename L, typename V, int N = 1>
	struct Allocate_With_Size {
		static void apply(Simulation_Datas& datas, L& d)
		{
			auto size = datas.get_data<V>();
			if (d.empty())
			{
				d.resize(size * N);
			}
		}
	};

	template<typename L,int Dim>
	struct Get_List_Size {
		static void apply(Simulation_Datas& datas, int& d)
		{
			d = datas.get_data<L>().size()/Dim;
		}
	};


	struct Delta { static void apply(Simulation_Datas& datas, std::vector<float>& d); };

	struct Compute_Edge_Length { static void apply(Simulation_Datas& datas, std::vector<float>& d); };

	struct Compute_Edge_Indices { static void apply(Simulation_Datas& datas, std::vector<int>& d); };


}


