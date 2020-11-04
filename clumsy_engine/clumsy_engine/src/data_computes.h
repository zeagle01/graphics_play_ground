

#pragma once


namespace clumsy_engine
{

	class Simulation_Datas;

	struct Copy_If_Empty {
		template<typename T>
		static void apply(const T& datas, T& d)
		{
			if (d.size() != datas.size())
			{
				d = datas;
			}
		}
	};

	struct Compute_Last_Frame_Position { static void apply(Simulation_Datas& datas, std::vector<float>& d); };
	struct Delta { static void apply(Simulation_Datas& datas, std::vector<float>& d); };
	struct Compute_Edge_Length { static void apply(Simulation_Datas& datas, std::vector<float>& d); };


}


