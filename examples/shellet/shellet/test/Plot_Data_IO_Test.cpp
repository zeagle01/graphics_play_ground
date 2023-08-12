


#include "gtest/gtest.h"
#include "Plot_Data_IO.h"
#include <algorithm>




TEST(Plot_Data_IO_Test,test_write_few_num){
    auto  io=Plot_Data_IO::get_instance();
    io->write_few_numbers<int>("test_write_few_num.txt",{1,2,3});

    std::vector<int> data;
    std::vector<int> data_shape;
    io->read_tensor("test_write_few_num.txt",data,data_shape);

    std::vector<int> exp_data{1,2,3};
    std::vector<int> exp_data_shape{3};
    EXPECT_EQ(data,exp_data);
    EXPECT_EQ(data_shape,exp_data_shape);
} 

TEST(Plot_Data_IO_Test,test_write_shaped_tensor){
    auto  io=Plot_Data_IO::get_instance();
    const int32_t n0=2,n1=2,n2=2;
    int32_t data_in[n0][n1][n2];
    for (int32_t i = 0; i < n0; i++)
    {
        for (int32_t j = 0; j < n1; j++)
        {
            for (int32_t k = 0; k < n2; k++)
            {
                data_in[i][j][k]=i*n1*n2+j*n2+k;
            }
        }
    }
    io->write_shaped_tensor<int>("test_write_shaped_tensor.txt",
    [&](const std::vector<int32_t>& index){
        return data_in[index[0]][index[1]][index[2]];
    },
    {n0,n1,n2});

    std::vector<int> data;
    std::vector<int> data_shape;
    io->read_tensor("test_write_shaped_tensor.txt",data,data_shape);

    std::vector<int> exp_data{0,1,2,3,4,5,6,7};
    std::vector<int> exp_data_shape{n0,n1,n2};
    EXPECT_EQ(data,exp_data);
    EXPECT_EQ(data_shape,exp_data_shape);
} 

TEST(Plot_Data_IO_Test,test_write_key_file){
    std::vector<float> X{0,0,0,0,1,0,1,1,0};
    std::vector<int32_t> EV{0,1,2};
    auto  io=Plot_Data_IO::get_instance();
    io->write_key_file<float>("test_write_key_file.k",Plot_Data_IO::Polygon_Type::triangle,X,EV);


    //todo:
    //read from file and make expectations

} 

TEST(Plot_Data_IO_Test,test_write_tecplot_triangle_mesh){
    std::vector<float> X{0,0,0,0,1,0,1,1,0};
    std::vector<int32_t> EV{0,1,2};
    auto  io=Plot_Data_IO::get_instance();
    io->write_tecplot_triangle_mesh<float>("test_write_tecplot_triangle_mesh.plt",X,EV);

    //todo:
    //read from file and make expectations
} 

TEST(Plot_Data_IO_Test,test_write_tecplot_structure_data_4_1){
    std::vector<float> data{
        0, 0,
        1, 0,
        2, 1,
        3, 1};
    auto  io=Plot_Data_IO::get_instance();
    io->write_tecplot_structure_data<float>("test_write_tecplot_structure_data_4_1.plt",&data[0],{4,1});

    //todo:
    //read from file and make expectations
} 

TEST(Plot_Data_IO_Test,test_write_tecplot_structure_data_2_2_1){
        std::vector<float> data{
            0, 0, 0,
            1, 0, 0,
            0, 1, 1,
            1, 1, 1};
        auto io = Plot_Data_IO::get_instance();
    io->write_tecplot_structure_data<float>("test_write_tecplot_structure_data_2_2_1.plt",&data[0],{2,2,1});

    //todo:
    //read from file and make expectations
}
