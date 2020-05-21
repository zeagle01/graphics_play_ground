

#pragma once

#include "test.h"
#include <vector>
#include <memory>

#include "glm/glm.hpp"

#include "shader.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "texture.h"

namespace test
{


	class Test_Texture2D: public Test
	{

	public:
		Test_Texture2D();
		virtual ~Test_Texture2D();

		void on_update(float deltaTime) override;
		void on_render() override;
		void on_imgui_render() override;
	private:
		std::vector<float> m_positions;
		std::vector<unsigned int> m_indices;

		std::vector<float> m_uniform_color;
		int m_frame;
		Shader m_shader;


		std::unique_ptr<Vertex_Buffer> m_vbo;
		std::unique_ptr<Index_Buffer> m_ibo;
		std::unique_ptr<Vertex_Array> m_va;

		std::unique_ptr<Texture> m_texture; 

		glm::mat4 m_proj ;
		glm::mat4 m_view ;
		std::vector<glm::vec3> m_model_translates ;


	};

}