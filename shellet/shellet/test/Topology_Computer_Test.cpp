



#include "Topology_Computer_Test.h"
#include <algorithm>
#include "gmock/gmock.h"

using namespace testing;




TEST_F(Topology_Computer_Test, test_compute_edge_of_one_triangle) {

	m3xi TV(3,1); TV<< 0, 1, 2;

	m2xi act_EV;

	//tc->compute_edge_indices(act_EV, TV);
	tc->precompute(TV);
	tc->get_edge_indices(act_EV);

	m2xi exp_EV(2,3);
	exp_EV << 
		1, 2, 0,
		0, 1, 2;

	//EXPECT_THAT(act_EV, Eq(exp_EV));



}

TEST_F(Topology_Computer_Test, test_compute_edge_of_2_by_2_square) {

	m3xi TV(3,2); 
	TV <<
		0, 0,
		1, 2,
		2, 3;

	//m2xi act_EV(2,3);
	m2xi act_EV;

	//tc->compute_edge_indices(act_EV, TV);

	tc->precompute(TV);
	tc->get_edge_indices(act_EV);

	m2xi exp_EV(2,5);
	exp_EV <<
		1, 2, 0, 3, 0,
		0, 1, 2, 2, 3;


	//EXPECT_EQ(act_EV, exp_EV);
}

TEST_F(Topology_Computer_Test, test_one_ring) {

	m3xi TV(3,2); 
	TV <<
		0, 0,
		1, 2,
		2, 3;

	//m2xi act_EV(2,3);
	//m2xi act_EV;

	//tc->compute_edge_indices(act_EV, TV);

	tc->precompute(TV);
	auto one_ring=tc->get_one_ring();

	std::vector<std::vector<int>> exp_one_ring{
		{1,2,3},
		{0,2},
		{0,1,3},
		{0,2}
	};
	
	for (int i = 0; i < exp_one_ring.size(); i++) {
		auto act_i = one_ring(i);
		std::sort(std::begin(act_i),std::end( act_i) );
		//EXPECT_EQ(act_i,exp_one_ring[i] );
	}
}
