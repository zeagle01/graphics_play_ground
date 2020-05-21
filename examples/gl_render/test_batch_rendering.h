


#pragma once

#include "Test.h"
#include "shader.h"
#include "texture.h" 


namespace test
{


	struct Vertex 
	{
		float position[3];
		float color[4];
		float texture_cood[2];
		float texture_unit;
	};

	struct Quad
	{
		Vertex vertices[4];
		unsigned int indices[6];
		Quad(float x, float y, float width,float height,float texture_unit)
			:vertices
		{
			{ {x - width/2, y - height/2, 0.0f}, { 0.2f,0.f,1.f,1.f }, { -1,-1 }, { texture_unit } },
			{ {x + width/2,y - height/2,1.0f},{0.f,0.5f,1.f,1.f},{1,-1},{texture_unit} },
			{ {x + width/2,y + height/2,1.0f},{0.1f,0.6f,1.f,1.f},{1,1},{texture_unit} },
			{ {x - width/2,y + height/2,0.0f},{0.9f,0.f,0.9f,1.f},{-1,1},{texture_unit} }
		},
			indices{
				0,1,2,
				2,3,0
				}

		{
		}
	};

	class Test_Batch_Rendering:public Test
	{
	public:
		Test_Batch_Rendering();
		virtual ~Test_Batch_Rendering();

		void on_update(float deltaTime) override;
		void on_render() override;
		void on_imgui_render() override;

	private:

		void push_quad(const Quad& quad);

		std::unique_ptr<Shader> m_shader;
		std::vector<std::unique_ptr<Texture>> m_textures;

		bool m_is_gpu_allocated;
		unsigned int m_vao;
		unsigned int m_vbo;
		unsigned int m_ebo;

		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;

		float m_translate[3];
		float m_color_translate[3];

		float m_texture_cood_translate[2];

		size_t m_row;
		size_t m_col;

	};

}