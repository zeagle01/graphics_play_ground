

#include "FEM_Simulator.h"

#include "predef_types.h"

#include <algorithm>

#include <Eigen/Eigen>

#include <Eigen/Geometry>

#include <iostream>


void FEM_Simulator::update(std::vector<float>& positions, std::vector<int>& triangle_indices) 
{

    compute_new_stretched_positions(positions);
}



void FEM_Simulator::setMesh(const std::vector<float>& positions, const std::vector<int>& triangle_indices) 
{

    size_t v_num=positions.size() / 3;

    m_positions = m3xf::Map(positions.data(), 3,v_num);
    m_init_positions=m_positions.block(0,0,2,v_num);

    m_init_positions(1, 2) = 1; //hand made value

    m_forces = m3xf::Zero(3, v_num);
    m_velocities = m3xf::Zero(3, v_num);
    m_indices = triangle_indices;


    m_delta_velocities = m3xf::Zero(3, v_num);
}

void FEM_Simulator::setGravity(const std::vector<float>& g) 
{

}

void FEM_Simulator::setMass(const float mass) 
{

}
void FEM_Simulator::setDeltaT(const float dt) 
{
    this->dt=dt;
}

void FEM_Simulator::compute_new_stretched_positions( std::vector<float>& x)
{
    size_t v_num=x.size() / 3;

    m_forces = m3xf::Zero(3, v_num);

    m_positions = m3xf::Map(x.data(), 3,v_num);

    size_t t_num = m_indices.size() / 3;
    std::vector<m22f> dm_list(t_num);
    std::vector<m22f> dm_inv_list(t_num);
    std::vector<m32f> F_list(t_num);
    for (size_t i=0;i<t_num;i++ )
    {
        m22f dm;
        dm.col(0) = m_init_positions.col(0) - m_init_positions.col(2);
        dm.col(1) = m_init_positions.col(1) - m_init_positions.col(2);

        dm_list[i] = dm;
        m22f dm_inv = dm.inverse();
        dm_inv_list[i] = dm_inv;

        m32f F;
        F.col(0) = m_positions.col(0) - m_positions.col(2);
        F.col(1) = m_positions.col(1) - m_positions.col(2);
        F = F * dm_inv;
        F_list[i] = F;

        m22f epsilon;
        epsilon = 0.5 * (F.transpose() * F - m22f::Identity());

        v3f vec_epsilon;
        vec_epsilon[0] = epsilon(0, 0);
        vec_epsilon[1] = epsilon(1, 1);
        vec_epsilon[2] = epsilon(0, 1);

        m33f material_coefficients;
        material_coefficients <<
            1, 1, 0,
            1, 1, 0,
            0, 0, 1;

        v3f vec_delta = material_coefficients * vec_epsilon;
        m22f delta;
        delta << vec_delta[0], vec_delta[2], vec_delta[2], vec_delta[1];

        float a = 0.5 * std::abs(dm.determinant());
        m33f f;
        f.block(0,0,3,2) = a * F * delta * dm_inv.transpose();
        f.col(2)=-f.col(0)-f.col(1);

        for (size_t j = 0; j < 3; j++)
        {
            size_t v = m_indices[i * 3 + j];
            m_forces.col(v) += f.col(j);
        }
    }
	m_velocities +=m_forces*dt;
    m_positions+=m_velocities*dt;
    x.assign(m_positions.data(),m_positions.data()+3*v_num);

}

void FEM_Simulator::implicit_method()
{

    using eigen_triplet = Eigen::Triplet<float>;
    std::vector<eigen_triplet> coo_list;

    size_t t_num = m_indices.size() / 3;
    for (size_t i = 0; i < t_num; i++)
    {
        //m33f m=
        //        v2i ev = simulation_data->edge_indices.col(ei);
        //        float w[]{1, -1};
        //        E_coefficients.push_back(eigen_triplet(ev[0], ei, w[0]));
        //        E_coefficients.push_back(eigen_triplet(ev[1], ei, w[1]));
    }
//    E.setFromTriplets(E_coefficients.begin(), E_coefficients.end());

}