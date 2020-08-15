
#include "Mesh_Loader_Test.h"


void Mesh_Loader_Test::SetUp(){
	mesh_loader=new Mesh_Loader();
}


TEST_F(Mesh_Loader_Test,read_1_triangle_without_texture){
    mesh_loader->load_from_obj("test_data/1_triangle.obj");
    std::vector<float> exp_pos{
	0,0,0,
	1,0,0,
	1,1,0
    };
    std::vector<int>exp_indices{
	0,1,2
    };
    ASSERT_EQ(mesh_loader->get_positions(),exp_pos);
    ASSERT_EQ(mesh_loader->get_indices(),exp_indices);
}

