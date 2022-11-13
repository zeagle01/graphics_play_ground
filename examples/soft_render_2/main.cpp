

#include "Drawing_Buffer.h"

#include <cmath>
#include <vector>
#include <array>


template<int Row,int Col>
struct mat
{
	//std::array< float, Row* Col> data;
	float data[Row * Col];

	float& operator()(int ri, int ci) { return data[ci * Row + ri]; }
	const float& operator() (int ri, int ci) const { return data[ci * Row + ri]; }

	float& operator()(int i) { return data[i]; }
	const float& operator() (int i) const { return data[i]; }

};


using mat4 = mat<4, 4>;
using vec3 = mat<3, 1>;
using vec4 = mat<4, 1>;


template<int Row,int N,int Col>
static mat<Row, Col> operator*(const mat<Row, N>& a, const mat<N, Col>& b)
{
	mat<Row, Col> ret;
	for (int ri = 0; ri < Row; ri++)
	{
		for (int ci = 0; ci < Col; ci++)
		{
			ret(ri, ci) = 0.f;
			for (int li = 0; li < N; li++)
			{
				ret(ri, ci) += a(ri, li) * b(li , ci);
			}
		}
	}
	return ret;
}


template<int Row,int Col>
static mat<Row, Col> operator+(const mat<Row, Col>& a, const mat<Row, Col>& b)
{
	mat<Row, Col> ret;
	for (int i = 0; i < Row * Col; i++)
	{
		ret.data[i] = a.data[i] + b.data[i];
	}
	return ret;
}

template<int Row,int Col>
static mat<Row, Col> operator-(const mat<Row, Col>& a, const mat<Row, Col>& b)
{
	mat<Row, Col> ret;
	for (int i = 0; i < Row * Col; i++)
	{
		ret.data[i] = a.data[i] - b.data[i];
	}
	return ret;
}

template<int Row,int Col>
static mat<Row, Col> operator*(float s, const mat<Row, Col>& a)
{
	mat<Row, Col> ret;
	for (int i = 0; i < Row * Col; i++)
	{
		ret.data[i] = s * a.data[i];
	}
	return ret;
}

template<int Row,int Col>
static mat<Row, Col> operator/(const mat<Row, Col>& a, float s)
{
	mat<Row, Col> ret;
	for (int i = 0; i < Row * Col; i++)
	{
		ret.data[i] = a.data[i] / s;
	}
	return ret;
}

static vec3 cross(const vec3& a, const vec3& b)
{
	vec3 ret;
	ret.data[0] = a.data[1] * b.data[2] - a.data[2] * b.data[1];
	ret.data[1] = -a.data[0] * b.data[2] + a.data[2] * b.data[0];
	ret.data[2] = a.data[0] * b.data[1] - a.data[1] * b.data[0];
	return ret;
}

template<int N>
static float dot(const mat<N, 1>& a,const mat<N, 1>& b)
{
	float ret;
	for (int i = 0; i < N; i++)
	{
		ret += a.data[i] * b.data[i];
	}
	return ret;
}

template<int N>
static float length(const mat<N, 1>& v)
{
	return std::sqrt(dot(v, v));
}

template<int N>
static mat<N, 1> normalize(const mat<N, 1>& v)
{
	return v / length(v);
}

template<int N>
static mat<N, N> get_identity()
{
	mat<N, N> ret;
	for (int i = 0; i < N; i++)
	{
		for(int j=0;j<N;j++)
		{
			if (i == j)
			{
				ret(i, j) = 1.f;
			}
			else
			{
				ret(i, j) = 0.f;
			}
		}
	}
	return ret;
}





struct Spinning_Cube
{

	int m_width;
	int m_height;
	Drawing_Buffer* screen;
	float m_aspect = float(m_width) / float(m_height);
	float cube_side = 100.f;
	float cube_unit = 1.2f;
	float m_world_height = 400.f;

	vec3 m_up{0,1,0};
	vec3 m_lookat{50,50,0};
	vec3 m_camara_location{ 600,600,100 };

	float m_angle_x = 0.f;
	float m_angle_y = 0.f;
	float m_angle_z = 0.f;


	mat4 get_rotation_matrix()
	{
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

	mat4 get_camara_matrix() 
	{

		vec3 z = normalize(m_camara_location - m_lookat);
		vec3 x = normalize(cross(m_up, z));
		vec3 y = normalize(cross(z, x));

		mat4 frame
		{
			{
				x.data[0],y.data[0],z.data[0],0,
				x.data[1],y.data[1],z.data[1],0,
				x.data[2],y.data[2],z.data[2],0,
				0,0,0,1,
			}
		};

		
		mat4 trans{
			{
				1,0,0,-m_camara_location.data[0],
				0,1,0,-m_camara_location.data[1],
				0,0,1,-m_camara_location.data[2],
				0,0,0,1.f
			}
		};


		return frame * trans;
	}

	mat4 get_scale_and_translate(const std::array<vec3, 2>& box_from, const std::array<vec3, 2>& box_dst)
	{
		mat4 translate_back_to_origin = get_identity<4>();

		translate_back_to_origin(0, 3) = -box_from[0](0);
		translate_back_to_origin(1, 3) = -box_from[0](1);
		translate_back_to_origin(2, 3) = -box_from[0](2);

		mat4 scale_back_to_cubic = get_identity<4>();
		auto dx_from = box_from[1] - box_from[0];

		scale_back_to_cubic(0, 0) = 1.f / dx_from(0);
		scale_back_to_cubic(1, 1) = 1.f / dx_from(1);
		scale_back_to_cubic(2, 2) = 1.f / dx_from(2);

		mat4 translate_to_dst = get_identity<4>();

		translate_to_dst(0, 3) = box_dst[0](0);
		translate_to_dst(1, 3) = box_dst[0](1);
		translate_to_dst(2, 3) = box_dst[0](2);

		mat4 scale_to_dst = get_identity<4>();
		auto dx_dst = box_dst[1] - box_dst[0];

		scale_to_dst(0, 0) =  dx_dst(0);
		scale_to_dst(1, 1) =  dx_dst(1);
		scale_to_dst(2, 2) =  dx_dst(2);

		return  translate_to_dst * scale_to_dst * scale_back_to_cubic * translate_back_to_origin;
	}

	mat4 to_pixel_space()
	{
		std::array<vec3, 2> screen_space_box{
			{
				{0,0,0},
				{float(m_width),float(m_height) ,1.f}
			}

		};

		std::array<vec3, 2> object_space_box{
			{
				{m_lookat(0) - m_aspect * m_world_height / 2  ,m_lookat(1) - m_world_height / 2 ,-1},
				{m_lookat(0) + m_aspect * m_world_height / 2 , m_lookat(1) + m_world_height / 2 ,1 }
			}

		};

		return  get_scale_and_translate(object_space_box, screen_space_box);
	}




	void compute_pixel(float x, float y, float z)
	{
		mat<4, 1> p{ {x,y,z,1.f} };
		auto rotx = get_rotation_matrix();
		//auto camara_matrix = get_camara_matrix();
		//p = camara_matrix * rotx * p;

		auto m = to_pixel_space();
		p = m * rotx* p;
		int xi = p(0);
		int yi = p(1);
		if (xi >= 0 && xi < m_width && yi >= 0 && yi < m_height)
		{
			screen->set_color(xi, yi, 0.2, 0.3, 0.5);
		}

	}

	void update()
	{

		for (float xi = 0.f; xi < cube_side; xi += cube_unit)
		{
			for (float yi = 0.f; yi < cube_side; yi += cube_unit)
			{
				compute_pixel(xi, yi, 0.f);
				compute_pixel(xi, yi, cube_side);
				compute_pixel(xi, 0, yi);
				compute_pixel(xi, cube_side, yi);
				compute_pixel(0, xi, yi);
				compute_pixel(cube_side, xi, yi);
			}
		}

		m_angle_x += 0.01;
		m_angle_y += 0.05;
		m_angle_z += 0.03;
	}


};


int main()
{
	Drawing_Buffer screen;

	int width = 800;
	int height = 600;

	screen.init(width, height);

	Spinning_Cube sc{ width,height, &screen };

	screen.main_loop([&screen, &sc, width, height]()
		{
			screen.clear();

			sc.update();

		});

	return 0;
}


