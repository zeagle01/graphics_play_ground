


#pragma once

#include "Test.h"
#include "shader.h"


namespace test
{


	struct Vertex 
	{
		float position[3];
		float color[4];
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

		std::unique_ptr<Shader> m_shader;

		bool m_is_gpu_allocated;
		unsigned int m_vao;
		unsigned int m_vbo;
		unsigned int m_ebo;

		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;

		float m_translate[3];
		float m_color_translate[3];

	};

}