



#pragma once

#include "math.h"

#include <iostream>
#include <algorithm>
#include <memory>
#include <thread>
#include <algorithm>
#include <Mesh_Loader.h>

#include "soft_render.h"
#include "frame_buffer.h"
#include "out_device.h"
#include "rasterizer.h"



namespace soft_render
{

#define set_color_fn(fb,r,g,b)  [&](int x, int y) {fb->set_color_f(x, y, {r,g,b,1.f}); } 


	class Plain_Renderer
	{
	public:
		Plain_Renderer() 
		{

			const int width = 800;
			const int height = 600;

			m_out_device = std::make_unique<Out_Device>();
			m_frame_buffer = std::make_unique<Array_Frame_Buffer>(width, height);
			m_rasterizer = std::make_unique<Rasterizer>();
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

				m_rasterizer->loop_line({ xa, y }, { xb, y }, [&](int x, int y) {m_frame_buffer->set_color_f(x, y, color); });
			}

			for (int y = v1.y; y < v2.y; y++)
			{
				float t1 = float(y - v1.y) / float(v2.y - v1.y);
				float t2 = float(y - v0.y) / float(v2.y - v0.y);
				int xa = v1.x + (v2.x - v1.x) * t1;
				int xb = v0.x + (v2.x - v0.x) * t2;

				m_rasterizer->loop_line({ xa, y }, { xb, y }, [&](int x, int y) {m_frame_buffer->set_color_f(x, y, color); });
				//m_rasterizer->line(xa, y, xb, y, color);
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

						//m_rasterizer->loop_line({ x0, y0 }, { x1, y1 }, [&](int x, int y) {m_frame_buffer->set_color_f(x, y, { 0.f,1.f,0.5f,1.f }); });
						tv.push_back({ x0,y0 });
					}
					m_rasterizer->triangle_fragment_loop(tv[0], tv[1], tv[2], [&](int x, int y) {m_frame_buffer->set_color_f(x, y, { 0.f,1.f,0.5f,1.f }); });
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
		std::map<int, vec3f > vec3_vars;
		std::map<int, vec4f > vec4_vars;
		//std::map<int, mat4x4 > mat_vars;
	};






	class PipeLine_Renderer
	{
	private:
		std::vector<vec3f> m_positions;
		std::vector<vec3i> m_indices;
	private:
		std::function <vec4f(Shader_Context&, Shader_Context&) > m_vertex_shader;
		std::function<vec4f(Shader_Context&)> m_fragment_shader;
		std::vector<Shader_Context> m_vertex_shader_in;
		std::vector<Shader_Context> m_vertex_shader_out;
		Shader_Context m_fragment_shader_in;

	private:
		std::unique_ptr<Rasterizer> m_rasterizer;
		std::unique_ptr<Frame_Buffer> m_frame_buffer;
		std::unique_ptr<Out_Device> m_out_device;
		int m_width, m_height;
		static constexpr int VAR_COLOR = 0;
		static constexpr int VAR_POSITION = 0;

	public: 
		PipeLine_Renderer() 
		{



			/////////////////////////////////////mesh
			Mesh_Loader mesh_loader;
			std::string mesh_dir = "../../../resources/meshes/";
			//mesh_loader.load_from_obj(mesh_dir + "african_head.obj");
			//mesh_loader.load_from_obj(mesh_dir+"flag.obj");
			mesh_loader.load_from_obj(mesh_dir+"1_triangle.obj");
			auto positions = mesh_loader.get_positions();
			auto indices = mesh_loader.get_indices();
			m_positions.resize(positions.size() / 3);
			m_indices.resize(indices.size() / 3);
			memcpy(&m_positions[0][0], &positions[0], sizeof(float) * positions.size());
			memcpy(&m_indices[0][0], &indices[0], sizeof(int) * indices.size());

			/////////////////////////////////////shader
			m_vertex_shader = [&](Shader_Context& vs_in, Shader_Context& fs_context)
			{
				return vs_in.vec4_vars[VAR_POSITION];
			};

			m_fragment_shader = [&](Shader_Context& fs_in)
			{
				return fs_in.vec4_vars[VAR_COLOR];
			};
			m_vertex_shader_in.resize(m_positions.size());
			m_vertex_shader_out.resize(m_positions.size());

			for (int i = 0; i < m_positions.size(); i++)
			{
				m_vertex_shader_in[i].vec3_vars[VAR_POSITION] = m_positions[i];
				m_vertex_shader_out[i].vec3_vars[VAR_COLOR] = m_positions[i];
			}

			m_fragment_shader_in.vec3_vars[VAR_COLOR] = vec3f();



			/////////////////////////////////frame
			m_width = 800;
			m_height = 600;

			m_out_device = std::make_unique<Out_Device>();
			m_frame_buffer = std::make_unique<Array_Frame_Buffer>(m_width, m_height);
			m_rasterizer = std::make_unique<Rasterizer>();
		}

		int* get_result() { return nullptr; }
		void set_vertex_shader() {}
		void render()
		{
			draw_triangle();
		}


		void draw_line(vec2i v0, vec2i v1)
		{}


		vec3f interpolate(vec2i v0, vec2i v1, vec2i v2, vec2i v)
		{
			vec2f dv0 = type_conversion<2,int,float>(v0 - v2);
			vec2f dv1 = type_conversion<2,int,float>(v1 - v2);
			vec2f b = type_conversion<2, int, float>(v - v2);
			mat2x2f A{ dv0,dv1 };

			vec2f x = solve(A, b);
			return vec3f{ x[0],x[1],1 - x[0] - x[1] };
		}

		void draw_triangle() 
		{
			vec2i pos_udc[3];
			Shader_Context vertex_shader_out[3];

			for (int ti = 0; ti < m_indices.size(); ti++)
			{
				for (int tvi = 0; tvi < 3; tvi++)
				{
					int vi = m_indices[ti][tvi];
					vec4f pos = m_vertex_shader(m_vertex_shader_in[vi], m_vertex_shader_out[vi]);
					pos_udc[tvi] = { int(pos[0] * m_width),int(pos[1] * m_height) };
					vertex_shader_out[tvi] = m_vertex_shader_out[vi];
				}
				for (int tvi = 0; tvi < 3; tvi++)
				{
					int tvi_next = (tvi + 1) % 3;
					m_rasterizer->loop_line(pos_udc[tvi], pos_udc[tvi_next], set_color_fn(m_frame_buffer, 1., 0.5, 0.2));
				}


				//m_rasterizer->triangle_fragment_loop(pos_udc[0], pos_udc[1], pos_udc[2], [&](int x,int y) 
				//	{
				//		vec3f w = interpolate(pos_udc[0], pos_udc[1], pos_udc[2], { x,y });

				//		for (auto& it : m_fragment_shader_in.float_vars)
				//		{
				//			int key = it.first;
				//			it.second = w[0] * vertex_shader_out[0].float_vars[key] + w[1] * vertex_shader_out[1].float_vars[key] + w[2] * vertex_shader_out[2].float_vars[key];
				//		}

				//		for (auto& it : m_fragment_shader_in.vec2_vars)
				//		{
				//			int key = it.first;
				//			it.second = w[0] * vertex_shader_out[0].vec2_vars[key] + w[1] * vertex_shader_out[1].vec2_vars[key] + w[2] * vertex_shader_out[2].vec2_vars[key];
				//		}

				//		for (auto& it : m_fragment_shader_in.vec3_vars)
				//		{
				//			int key = it.first;
				//			it.second = w[0] * vertex_shader_out[0].vec3_vars[key] + w[1] * vertex_shader_out[1].vec3_vars[key] + w[2] * vertex_shader_out[2].vec3_vars[key];
				//		}


				//		vec4f color = m_fragment_shader(m_fragment_shader_in);

				//		m_frame_buffer->set_color_f(x, y, color);
				//	}
				//);

			}

			m_out_device->flush(*m_frame_buffer);

		}
	};






}