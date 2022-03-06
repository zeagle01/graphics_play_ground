

#include "renderer_2D.h"
#include "clumsy_engine.h"

#include <vector>

namespace clumsy_engine
{

	struct Render_2D_Storage
	{
		clumsy_engine::Ref<clumsy_engine::Shader> m_shader_plane;
		clumsy_engine::Ref<clumsy_engine::Vertex_Array> m_vertex_array_plane;
	};

	static  Render_2D_Storage* s_data;

	void Renderer_2D::init()
	{
		s_data = new Render_2D_Storage();

		std::vector<float> m_positions_plane = {
			0.,0,0,
			1,0,0,
			1,1,0,
			0,1,0
		};

		std::vector<int> triangles{
			0,1,2,
			0,2,3
		};

		//shader

		std::string resources_dir = "../../../resources/";
		s_data->m_shader_plane = Shader::create(resources_dir + "shaders/flat_plane_2d.glsl");

		//gl data stuff
		s_data->m_vertex_array_plane = Vertex_Array::create();
		std::string position_name_in_shader = "position";
		int v_num = m_positions_plane.size() / 3;

		s_data->m_vertex_array_plane->add_vertex_attribute(s_data->m_shader_plane->get_id(), Shader_Data_Type::Float3, position_name_in_shader);
		s_data->m_vertex_array_plane->set_vertex_attribute_data(position_name_in_shader, m_positions_plane.data(), v_num);

		Ref<Index_Buffer> index_buffer = Index_Buffer::create(triangles.data(), triangles.size());
		s_data->m_vertex_array_plane->set_index_buffer(index_buffer);
	}

	void Renderer_2D::shutdown()
	{
		delete s_data;
	}

	void Renderer_2D::begin_scene(Ref<Camara> camara)
	{
		s_data->m_shader_plane->bind();
		s_data->m_shader_plane->upload_uniform_mat4("u_view_projection", camara->get_view_projection_matrix());
	}

	void	Renderer_2D::end_scene()
	{

	}

	//
	void Renderer_2D::draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		draw_quad({ position.x,position.y,0.f }, size, color);
	}
	void Renderer_2D::draw_quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{

		s_data->m_shader_plane->bind();
		s_data->m_shader_plane->upload_uniform_vec4("u_color", color);

		auto model_matrix = glm::mat4(1.f);
		model_matrix = glm::translate(model_matrix, glm::vec3(position.x, position.y, 0.f));
		model_matrix = glm::scale(model_matrix, glm::vec3(size.x, size.y, 1.f));

		s_data->m_shader_plane->upload_uniform_mat4("u_model_matrix", model_matrix);

		s_data->m_vertex_array_plane->bind();
		Render_Command::draw_indexed(s_data->m_vertex_array_plane);
	}
}
