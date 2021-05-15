
#include "renderer.h"
#include "render_command.h"
#include "vertex_array.h"
#include "shader.h"
#include "camara.h"

namespace clumsy_engine
{

	std::shared_ptr<Renderer::SceneData> Renderer::s_scene_data = std::make_shared<Renderer::SceneData>();

	void Renderer::begin_scene(std::shared_ptr<Orthorgraphic_Camara> camara)
	{
		s_scene_data->view_projection_matrix = camara->get_view_projection_matrix();
	}
	void Renderer::end_scene()
	{

	}
	void Renderer::submit(std::shared_ptr<Shader> shader, std::shared_ptr<Vertex_Array> vertex_array, const glm::mat4& transform)
	{
		shader->bind();
		shader->upload_uniform_mat4("u_view_projection", s_scene_data->view_projection_matrix);
		shader->upload_uniform_mat4("u_model_matrix", transform);
		vertex_array->bind();
		Render_Command::draw_indexed(vertex_array);
	}
}