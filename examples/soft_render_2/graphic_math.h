
#pragma once

#include "mat.h"

namespace soft_render
{

	template<typename T ,int N>
	static mat<T, N + 1, 1> get_homogeneous_point(const mat<T, N, 1>& v)
	{
		mat<T, N + 1, 1> ret;
		for (int i = 0; i < N; i++)
		{
			ret(i) = v(i);
		}
		ret(N) = 1.f;
		return ret;
	}

	template<typename T ,int N>
	static mat<T, N - 1, 1> from_homogeneous_point(const mat<T, N, 1>& v)
	{
		mat<T, N - 1, 1> ret;
		for (int i = 0; i < N - 1; i++)
		{
			ret(i) = v(i) / v(N - 1);
		}
		return ret;
	}

	template<typename T ,int N>
	static mat<T, N + 1, 1> get_homogeneous_vector(const mat<T, N, 1>& v)
	{
		mat<T, N + 1, 1> ret;
		for (int i = 0; i < N; i++)
		{
			ret(i) = v(i);
		}
		ret(N) = 0.f;
		return ret;
	}

	template<typename T ,int N>
	static mat<T, N - 1, 1> from_homogeneous_vector(const mat<T, N, 1>& v)
	{
		mat<T, N - 1, 1> ret;
		for (int i = 0; i < N - 1; i++)
		{
			ret(i) = v(i);
		}
		return ret;
	}


	//tranformation
	static mat4 get_translate_matrix(const vec3& translate)
	{
		mat4 ret
		{
				1,0,0,translate(0),
				0,1,0,translate(1),
				0,0,1,translate(2),
				0,0,0,1
		};
		return  ret;
	}

	static mat4 get_rotation_matrix(const vec3& theta)
	{
		float m_angle_x = theta(0);
		float m_angle_y = theta(1);
		float m_angle_z = theta(2);

		float cx = std::cos(m_angle_x);
		float sx = std::sin(m_angle_x);
		mat4 rotx
		{
			{
				1,0,0,0,
				0,cx,-sx,0,
				0,sx,cx,0,
				0,0,0,1
			}
		};

		float cy = std::cos(m_angle_y);
		float sy = std::sin(m_angle_y);
		mat4 roty
		{
			{
				cy,0,-sy,0,
				0,1,0,0,
				sy,0,cy,0,
				0,0,0,1
			}
		};

		float cz = std::cos(m_angle_z);
		float sz = std::sin(m_angle_z);
		mat4 rotz
		{
			{
				cz,-sz,0,0,
				sz,cz,0,0,
				0,0,1,0,
				0,0,0,1
			}
		};

		return rotz * roty * rotx;
	}

	static mat4 get_scale_matrix(const vec3& s)
	{
		return mat4 
		{
			{
				s(0),	0,		0,		0,
				0,		s(1),	0,		0,
				0,		0,		s(2),	0,
				0,		0,		0,		1
			}
		};
	}


	static mat4 get_projection_matrix(float near,float far)
	{
		mat4 ret
		{
			near,0,0,0,
			0,near,0,0,
			0,0,near + far,-near * far,
			0,0,1,0
		};

		return ret;
	}

	static mat4 get_box_transform_matrix(const std::array<vec3, 2>& box_from, const std::array<vec3, 2>& box_dst)
	{
		mat4 translate_back_to_origin = get_identity<float,4>();

		translate_back_to_origin(0, 3) = -box_from[0](0);
		translate_back_to_origin(1, 3) = -box_from[0](1);
		translate_back_to_origin(2, 3) = -box_from[0](2);

		auto dx_from = box_from[1] - box_from[0];


		mat4 translate_to_dst = get_identity<float,4>();

		translate_to_dst(0, 3) = box_dst[0](0);
		translate_to_dst(1, 3) = box_dst[0](1);
		translate_to_dst(2, 3) = box_dst[0](2);

		mat4 scale_to_dst = get_identity<float,4>();
		auto dx_dst = box_dst[1] - box_dst[0];

		scale_to_dst(0, 0) = dx_dst(0) / dx_from(0);
		scale_to_dst(1, 1) = dx_dst(1) / dx_from(1);
		scale_to_dst(2, 2) = dx_dst(2) / dx_from(2);

		return  translate_to_dst * scale_to_dst * translate_back_to_origin;
	}


}
