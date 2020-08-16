



#pragma once

#include "math.h"

#include <iostream>
#include <algorithm>
#include <thread>
#include <Mesh_Loader.h>

#include "soft_render.h"
#include "frame_buffer.h"
#include "out_device.h"
#include "rasterizer.h"



namespace soft_render
{


	class Plain_Renderer
	{
	public:
		Plain_Renderer() 
		{
		}

		void render()
		{
			Mesh_Loader mesh_loader;
			std::string mesh_dir = "../../../resources/meshes/";
			mesh_loader.load_from_obj(mesh_dir + "african_head.obj");
			//mesh_loader.load_from_obj(mesh_dir+"flag.obj");
			//mesh_loader.load_from_obj(mesh_dir+"1_triangle.obj");
			auto positions = mesh_loader.get_positions();
			auto indices = mesh_loader.get_indices();

			const int width = 800;
			const int height = 600;

			Array_Frame_Buffer frame_buffer(width, height);
			Out_Device out_device;
			Rasterizer raterizer(frame_buffer);

			float t = 0.f;
			//	while (true)
			//	{
			system("cls");

			for (size_t ti = 0; ti < indices.size() / 3; ti++)
			{
				std::vector<vec2i> tv;
				for (int vi = 0; vi < 3; vi++)
				{
					int vi_next = (vi + 1) % 3;
					int v = indices[ti * 3 + vi];
					int v_next = indices[ti * 3 + vi_next];

					int x0 = (positions[v * 3 + 0] + 1.0) * width * 0.5;
					int y0 = (1.f - positions[v * 3 + 1]) * height * 0.5;
					int x1 = (positions[v_next * 3 + 0] + 1.0) * width * 0.5;
					int y1 = (1.f - positions[v_next * 3 + 1]) * height * 0.5;

					//line(x0, y0, x1, y1, RGB(255, 100, 0), my_dc);
					raterizer.line(x0, y0, x1, y1, vec4f{ 0.f,1.f,0.5f,1.f });
					tv.push_back({ x0,y0 });
				}
				//fill_triangle(tv[0], tv[1], tv[2], RGB(100, 155, 80), my_dc);
			}

			t += .01;

			//		using namespace std::literals;
			//		std::this_thread::sleep_for(2000ms);
			//	}

			out_device.flush(frame_buffer);
		}

	private:
		//Rasterizer m_rasterizer;
	};


	//////////////////soft renderer
	struct Shader_Context
	{
		std::map<int, float > float_vars;
		std::map<int, vec2f > vec2_vars;
		std::map<int, vec2f > vec3_vars;
		std::map<int, vec4f > vec4_vars;
		//std::map<int, mat4x4 > mat_vars;
	};






	class Soft_Renderer
	{
	private:
		std::vector<vec3f> m_positions;
		std::vector<vec3i> m_indices;
		std::function<vec4f(int vi, Shader_Context&)> m_vertex_shader;
		std::function<vec4f(Shader_Context&)> m_fragment_shader;


	public: 
		Soft_Renderer() 
		{


			constexpr int VAR_COLOR = 0;

			m_vertex_shader = [&](int vi, Shader_Context& vs_io)
			{
				return vec4f({ m_positions[vi][0],m_positions[vi][1],m_positions[vi][2],1 });
			};

			m_fragment_shader = [&](Shader_Context& fs_in)
			{
				return fs_in.vec4_vars[VAR_COLOR];
			};
		}

		int* get_result() { return nullptr; }
		void set_vertex_shader() {}
		void render(){}

		void draw_line() 
		{  
		
		}

		void draw_line(int x0,int y0,int x1,int y1);

		void draw_triangle() 
		{
			Shader_Context vs_in[3];
			for (int ti = 0; ti < m_indices.size(); ti++)
			{
				for (int tvi = 0; tvi < 3; tvi++)
				{
					int vi = m_indices[ti][tvi];
					vec4f pos = m_vertex_shader(vi, vs_in[tvi]);
				}




			}

		}
	};






}