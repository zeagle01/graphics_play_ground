
#include "gmock/gmock.h"
#include "discrete_collision_detection.h"

using namespace testing;


TEST(Discrete_Collision_Detection_Test,ee_test)
{

	auto type_map = std::make_shared<clumsy_lib::Type_Map<void>>();

	clumsy_engine::Discrete_Collision_Detection detector;

	detector.set_type_map(type_map);

	detector.set_value<clumsy_engine::data::Position>(
		{
			{ 0,0,0 },
			{ 1,0,0 },
			{ 1,1,0 }
		}
		);

	detector.set_value<clumsy_engine::data::Edge_Indice>( { 0,1,1,2,2,0 } );

	//detector.construct_ee_bvh();
	detector.construct_ee_bvh1();
	detector.detection();


}





