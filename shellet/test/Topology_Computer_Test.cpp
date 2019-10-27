



#include "Topology_Computer_Test.h"




TEST_F(Topology_Computer_Test, test_compute_edge_of_one_triangle) {

	m3xi TV(3,1); TV<< 0, 1, 2;

	m2xi act_EV(2,3);

	tc->compute_edge_indices(act_EV, TV);

	m2xi exp_EV(2,3);
	//exp_EV << 1, 0, 2, 1, 0, 2 ;
	exp_EV << 
		1, 2, 0,
		0, 1, 2;

	EXPECT_EQ(act_EV, exp_EV);



}

TEST_F(Topology_Computer_Test, test_compute_edge_of_2_by_2_square) {

	m3xi TV(3,2); 
	TV <<
		0, 0,
		1, 2,
		2, 3;

	m2xi act_EV(2,3);

	tc->compute_edge_indices(act_EV, TV);

	m2xi exp_EV(2,5);
	exp_EV <<
		1, 2, 0, 3, 0,
		0, 1, 2, 2, 3;


	EXPECT_EQ(act_EV, exp_EV);
}
