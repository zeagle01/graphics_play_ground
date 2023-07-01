

#include "renderer_2D.h"
#include "clumsy_engine.h"
#include "texture.h"
#include "texture.h"

#include <vector>

namespace clumsy_engine
{

	struct Render_2D_Storage
	{
		clumsy_engine::Ref<clumsy_engine::Shader> m_shader_texture;
		clumsy_engine::Ref<clumsy_engine::Vertex_Array> m_vertex_array_plane;
		clumsy_engine::Ref<clumsy_engine::Texture_2D> m_white_texture;
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

		std::vector<float> m_texture_cood_plane = {
			0.f,0.f,
			1.f,0.f,
			1.f,1.f,
			0.f,1.f
		};

		std::vector<int> triangles{
			0,1,2,
			0,2,3
		};

		//shader

		std::string resources_dir = "../../../resources/";

		//gl data stuff
		s_data->m_vertex_array_plane = Vertex_Array::create();
		int v_num = m_positions_plane.size() / 3;

		std::string position_name_in_shader = "position";
		s_data->m_vertex_array_plane->add_vertex_attribute(Shader_Data_Type::Float3, position_name_in_shader);
		s_data->m_vertex_array_plane->set_vertex_attribute_data(position_name_in_shader, m_positions_plane.data(), v_num);

		std::string texture_coodinate_name_in_shader = "texture_coodinate";
		s_data->m_vertex_array_plane->add_vertex_attribute(Shader_Data_Type::Float2, texture_coodinate_name_in_shader);
		s_data->m_vertex_array_plane->set_vertex_attribute_data(texture_coodinate_name_in_shader, m_texture_cood_plane.data(), v_num);

		Ref<Index_Buffer> index_buffer = Index_Buffer::create(triangles.data(), triangles.size());
		s_data->m_vertex_array_plane->set_index_buffer(index_buffer);

		s_data->m_shader_texture = Shader::create(resources_dir + "shaders/flat_plane_texture_2d.glsl");
		s_data->m_shader_texture->bind();
		s_data->m_shader_texture->upload_uniform_int("u_texture", 0);

		s_data->m_white_texture = Texture_2D::create(1, 1);
		uint32_t white_pixel = 0xffffffff;
		s_data->m_white_texture->set_data(&white_pixel, sizeof(white_pixel));
	}

	void Renderer_2D::shutdown()
	{
		delete s_data;
	}

	void Renderer_2D::begin_scene(Ref<Camara> camara)
	{
		s_data->m_shader_texture->bind();
		s_data->m_shader_texture->upload_uniform_mat4("u_view_projection", camara->get_view_projection_matrix());
	}

	void Renderer_2D::end_scene()
	{

	}

	//
	void Renderer_2D::draw_quad(const glm::vec2& position, const glm::vec2& size,  const glm::vec4& color)
	{
		draw_quad({ position.x,position.y,0.f }, size,  color);
	}
	void Renderer_2D::draw_quad(const glm::vec3& position, const glm::vec2& size,  const glm::vec4& color)
	{

		s_data->m_shader_texture->bind();
		s_data->m_shader_texture->upload_uniform_vec4("u_color", color);
		s_data->m_shader_texture->upload_uniform_float("u_tilingFactor", 1.f);

		s_data->m_white_texture->bind();

		auto model_matrix = glm::mat4(1.f);
		model_matrix = glm::translate(model_matrix, glm::vec3(position.x, position.y, 0.f));
		model_matrix = glm::scale(model_matrix, glm::vec3(size.x, size.y, 1.f));

		s_data->m_shader_texture->upload_uniform_mat4("u_model_matrix", model_matrix);

		s_data->m_vertex_array_plane->bind();
		Render_Command::draw_indexed(s_data->m_vertex_array_plane);
	}

	void Renderer_2D::draw_rotated_quad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{

		s_data->m_shader_texture->bind();
		s_data->m_shader_texture->upload_uniform_vec4("u_color", color);
		s_data->m_shader_texture->upload_uniform_float("u_tilingFactor", 1.f);

		s_data->m_white_texture->bind();

		auto model_matrix = glm::mat4(1.f);
		model_matrix = glm::translate(model_matrix, glm::vec3(position.x, position.y, 0.f));
		model_matrix = glm::rotate(model_matrix, rotation, { 0.f, 0.f, 1.f });
		model_matrix = glm::scale(model_matrix, glm::vec3(size.x, size.y, 1.f));

		s_data->m_shader_texture->upload_uniform_mat4("u_model_matrix", model_matrix);

		s_data->m_vertex_array_plane->bind();
		Render_Command::draw_indexed(s_data->m_vertex_array_plane);

	}

	void Renderer_2D::draw_rotated_quad(const glm::vec3& position, const glm::vec2& size, float rotation, Ref<Texture_2D> texture, float tilling_factor, const glm::vec4& tint_color)
	{
		s_data->m_shader_texture->bind();
		texture->bind(0);

		s_data->m_shader_texture->upload_uniform_vec4("u_color", tint_color);
		s_data->m_shader_texture->upload_uniform_float("u_tilingFactor", tilling_factor);

		auto model_matrix = glm::mat4(1.f);
		model_matrix = glm::translate(model_matrix, glm::vec3(position.x, position.y, 0.f));
		model_matrix = glm::rotate(model_matrix, rotation, { 0.f, 0.f, 1.f });
		model_matrix = glm::scale(model_matrix, glm::vec3(size.x, size.y, 1.f));

		s_data->m_shader_texture->upload_uniform_mat4("u_model_matrix", model_matrix);

		s_data->m_vertex_array_plane->bind();
		Render_Command::draw_indexed(s_data->m_vertex_array_plane);

	}

	void Renderer_2D::draw_rotated_quad(const glm::vec2& position, const glm::vec2& size, float rotation, Ref<Texture_2D> texture, float tilling_factor, const glm::vec4& tint_color)
	{
		draw_rotated_quad({ position.x,position.y,0.f }, size, rotation, texture, tilling_factor, tint_color);
	}

	void Renderer_2D::draw_rotated_quad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		draw_rotated_quad({ position.x,position.y,0.f }, size, rotation, color);
	}

	void Renderer_2D::draw_quad(const glm::vec2& position, const glm::vec2& size, Ref<Texture_2D> texture, float tilling_factor, const glm::vec4& tint_color)
	{
		draw_quad({ position.x,position.y,0.f }, size, texture, tilling_factor, tint_color);
	}

	void Renderer_2D::draw_quad(const glm::vec3& position, const glm::vec2& size, Ref<Texture_2D> texture, float tilling_factor, const glm::vec4& tint_color)
	{

		s_data->m_shader_texture->bind();
		texture->bind(0);

		s_data->m_shader_texture->upload_uniform_vec4("u_color", tint_color);
		s_data->m_shader_texture->upload_uniform_float("u_tilingFactor", tilling_factor);

		auto model_matrix = glm::mat4(1.f);
		model_matrix = glm::translate(model_matrix, glm::vec3(position.x, position.y, 0.f));
		model_matrix = glm::scale(model_matrix, glm::vec3(size.x, size.y, 1.f));

		s_data->m_shader_texture->upload_uniform_mat4("u_model_matrix", model_matrix);


		s_data->m_vertex_array_plane->bind();
		Render_Command::draw_indexed(s_data->m_vertex_array_plane);

	}
}
