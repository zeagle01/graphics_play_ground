
#include "renderer.h"
#include "render_command.h"
#include "vertex_array.h"
#include "shader.h"
#include "openGL_shader.h"
#include "camara.h"
#include "profiler.h"

namespace clumsy_engine
{

	Ref<Renderer::SceneData> Renderer::s_scene_data = std::make_shared<Renderer::SceneData>();

	void Renderer::begin_scene(Ref<Orthorgraphic_Camara> camara)
	{
		s_scene_data->view_projection_matrix = camara->get_view_projection_matrix();
	}
	void Renderer::end_scene()
	{

	}
	void Renderer::submit(Ref<Shader> shader, Ref<Vertex_Array> vertex_array, const glm::mat4& transform)
	{
		RECORD_FUNCTION_DURATION();

		auto ogl_shader = std::dynamic_pointer_cast<OpenGL_Shader>(shader);
		if (ogl_shader)
		{
			ogl_shader->bind();
			ogl_shader->upload_uniform_mat4("u_view_projection", s_scene_data->view_projection_matrix);
			ogl_shader->upload_uniform_mat4("u_model_matrix", transform);
			vertex_array->bind();
			Render_Command::draw_indexed(vertex_array);
		}
	}
}