



#pragma once

#include "math.h"

#include <iostream>
#include <algorithm>
#include <memory>
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

			const int width = 800;
			const int height = 600;

			m_out_device = std::make_unique<Out_Device>();
			m_frame_buffer = std::make_unique<Array_Frame_Buffer>(width, height);
			m_rasterizer = std::make_unique<Rasterizer>(*m_frame_buffer);
		}

		void fill_triangle(vec2i v0, vec2i v1, vec2i v2, vec4f color )
		{
			if (v0.y > v1.y) std::swap(v0, v1);
			if (v0.y > v2.y) std::swap(v0, v2);
			if (v1.y > v2.y) std::swap(v1, v2);
			//int total_height
			for (int y = v0.y; y < v1.y; y++)
			{
				float t1 = float(y - v0.y) / float(v1.y - v0.y);
				float t2 = float(y - v0.y) / float(v2.y - v0.y);
				int xa = v0.x + (v1.x - v0.x) * t1;
				int xb = v0.x + (v2.x - v0.x) * t2;

				m_rasterizer->line(xa, y, xb, y, color);
			}

			for (int y = v1.y; y < v2.y; y++)
			{
				float t1 = float(y - v1.y) / float(v2.y - v1.y);
				float t2 = float(y - v0.y) / float(v2.y - v0.y);
				int xa = v1.x + (v2.x - v1.x) * t1;
				int xb = v0.x + (v2.x - v0.x) * t2;

				m_rasterizer->line(xa, y, xb, y, color);
			}

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



			float t = 0.f;
			while (true)
			{
				system("cls");

				for (size_t ti = 0; ti < indices.size() / 3; ti++)
				{
					std::vector<vec2i> tv;
					for (int vi = 0; vi < 3; vi++)
					{
						int vi_next = (vi + 1) % 3;
						int v = indices[ti * 3 + vi];
						int v_next = indices[ti * 3 + vi_next];

						int x0 = (positions[v * 3 + 0] + 1.0) * m_frame_buffer->get_width() * 0.5;
						int y0 = (1.f - positions[v * 3 + 1]) * m_frame_buffer->get_height() * 0.5;
						int x1 = (positions[v_next * 3 + 0] + 1.0) * m_frame_buffer->get_width() * 0.5;
						int y1 = (1.f - positions[v_next * 3 + 1]) * m_frame_buffer->get_height() * 0.5;

						//line(x0, y0, x1, y1, RGB(255, 100, 0), my_dc);
						m_rasterizer->line(x0, y0, x1, y1, { 0.f,1.f,0.5f,1.f });
						tv.push_back({ x0,y0 });
					}
					//fill_triangle(tv[0], tv[1], tv[2], { 1.f,1.f,0.5f,1.f });
				}

				t += .01;

				m_out_device->flush(*m_frame_buffer);
			}

		}

	private:
		std::unique_ptr<Rasterizer> m_rasterizer;
		std::unique_ptr<Frame_Buffer> m_frame_buffer;
		std::unique_ptr<Out_Device> m_out_device;
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