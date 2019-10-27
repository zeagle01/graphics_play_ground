


#ifndef PREDEF_TYPES_H
#define PREDEF_TYPES_H
#include "Eigen/Dense"


typedef Eigen::Vector3f v3f;
typedef Eigen::Vector3i v3i;

typedef Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic> mxxf;
typedef Eigen::Matrix<int,Eigen::Dynamic,Eigen::Dynamic> mxxi;

typedef Eigen::Matrix<float,4,Eigen::Dynamic> m4xf;
typedef Eigen::Matrix<int,4,Eigen::Dynamic> m4xi;

typedef Eigen::Matrix<float,3,Eigen::Dynamic> m3xf;
typedef Eigen::Matrix<int,3,Eigen::Dynamic> m3xi;

typedef Eigen::Matrix<float,2,Eigen::Dynamic> m2xf;
typedef Eigen::Matrix<int,2,Eigen::Dynamic> m2xi;

typedef Eigen::VectorXf vxf;
typedef Eigen::VectorXi vXi;






#endif