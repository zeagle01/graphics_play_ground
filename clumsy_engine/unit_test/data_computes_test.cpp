

#include "gmock/gmock.h"

#include "simulation_data.h"



using namespace testing;
using namespace clumsy_engine;






TEST(Data_Computes_Test, compute_edge_indices)
{

	Simulation_Data_Acc<type_list<data::Triangle_Indice, data::Vertex_Num>> sim_data;
	sim_data.set<data::Triangle_Indice>({ 0,1,2 });
	sim_data.set<data::Vertex_Num>(3);

	std::vector<int> act;
	Compute_Edge_Indices::apply(sim_data, act);

	std::vector<int> exp{ 0,1,1,2,2,0 };

	EXPECT_THAT(act, Eq(exp));

}


TEST(Data_Computes_Test, compute_triangle_area)
{

	Simulation_Data_Acc<type_list<data::Triangle_Indice, data::Position>> sim_data;
	sim_data.set<data::Triangle_Indice>({ 0,1,2 });
	sim_data.set<data::Position>(
		{
			{0,0,0},
			{1,0,0},
			{1,1,0}
		}
	);

	std::vector<float> act;
	Compute_Triangle_Area::apply(sim_data, act);

	std::vector<float> exp{ 0.5f };

	EXPECT_THAT(act, Eq(exp));
}

TEST(Data_Computes_Test, compute_vertex_area)
{

	Simulation_Data_Acc<type_list<data::Vertex_Num, data::Triangle_Indice, data::Triangle_Area >> sim_data;

	sim_data.set<data::Triangle_Indice>({ 0,1,2 });
	sim_data.set<data::Triangle_Area>({3.f});
	sim_data.set<data::Vertex_Num>(3);

	std::vector<float> act;
	Compute_Vertex_Area::apply(sim_data, act);

	std::vector<float> exp{ 1,1,1 };

	EXPECT_THAT(act, Eq(exp));
}

