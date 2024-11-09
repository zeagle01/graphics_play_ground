

#include "renderer_2D.h"
#include "clumsy_engine.h"
#include "texture.h"
#include "texture.h"

#include <vector>
#include <algorithm>
#include <numeric>

namespace clumsy_engine
{

	struct Render_2D_Storage
	{
		const uint32_t max_quads = 10000;
		const uint32_t max_vertices = max_quads * 4;
		const uint32_t max_indices = max_quads * 6;
		const uint32_t max_texture_slots = 32;

		clumsy_engine::Ref<clumsy_engine::Shader> m_shader_texture;
		clumsy_engine::Ref<clumsy_engine::Vertex_Array> m_vertex_array_plane;
		clumsy_engine::Ref<clumsy_engine::Texture_2D> m_white_texture;

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> texcoods;
		std::vector<glm::vec4> colors;
		std::vector<float> texture_index;
		std::vector<float> tilling_factor;

		std::vector<clumsy_engine::Ref<clumsy_engine::Texture_2D>>  m_textures;

		uint32_t current_quad = 0;
		uint32_t current_texture_slot = 1; //texture 0 is reserved for white texture 

		Renderer_2D::Statistics statistics;
	};

	static  Render_2D_Storage s_data;

	static const std::string position_name_in_shader = "position";
	static const std::string color_name_in_shader = "color";
	static const std::string texture_coodinate_name_in_shader = "texture_coodinate";
	static const std::string texture_index_name_in_shader = "texture_index";
	static const std::string tilling_factor_name_in_shader = "tilling_factor";

	void Renderer_2D::init()
	{
		s_data.positions.resize(s_data.max_vertices);
		s_data.texcoods.resize(s_data.max_vertices);
		s_data.colors.resize(s_data.max_vertices);
		s_data.texture_index.resize(s_data.max_vertices);
		s_data.tilling_factor.resize(s_data.max_vertices);

		s_data.m_textures.resize(s_data.max_texture_slots);

		//shader
		std::string resources_dir = "../../../resources/";

		//gl data stuff
		s_data.m_vertex_array_plane = Vertex_Array::create();

		s_data.m_vertex_array_plane->add_vertex_attribute(Shader_Data_Type::Float3, position_name_in_shader);
		s_data.m_vertex_array_plane->set_vertex_attribute_data(position_name_in_shader, nullptr, s_data.max_vertices);

		s_data.m_vertex_array_plane->add_vertex_attribute(Shader_Data_Type::Float4, color_name_in_shader);
		s_data.m_vertex_array_plane->set_vertex_attribute_data(color_name_in_shader, nullptr, s_data.max_vertices);

		s_data.m_vertex_array_plane->add_vertex_attribute(Shader_Data_Type::Float2, texture_coodinate_name_in_shader);
		s_data.m_vertex_array_plane->set_vertex_attribute_data(texture_coodinate_name_in_shader, nullptr, s_data.max_vertices);

		s_data.m_vertex_array_plane->add_vertex_attribute(Shader_Data_Type::Float, texture_index_name_in_shader);
		s_data.m_vertex_array_plane->set_vertex_attribute_data(texture_index_name_in_shader, nullptr, s_data.max_vertices);

		s_data.m_vertex_array_plane->add_vertex_attribute(Shader_Data_Type::Float, tilling_factor_name_in_shader);
		s_data.m_vertex_array_plane->set_vertex_attribute_data(tilling_factor_name_in_shader, nullptr, s_data.max_vertices);

		std::vector<int> triangles(s_data.max_indices);
		int quadV = 0;
		for (int i = 0; i < s_data.max_quads; i++)
		{
			triangles[i * 6 + 0] = quadV + 0;
			triangles[i * 6 + 1] = quadV + 1;
			triangles[i * 6 + 2] = quadV + 2;

			triangles[i * 6 + 3] = quadV + 0;
			triangles[i * 6 + 4] = quadV + 2;
			triangles[i * 6 + 5] = quadV + 3;
			quadV += 4;
		}

		Ref<Index_Buffer> index_buffer = Index_Buffer::create(triangles.data(), triangles.size());
		s_data.m_vertex_array_plane->set_index_buffer(index_buffer);

		s_data.m_shader_texture = Shader::create(resources_dir + "shaders/flat_plane_texture_2d.glsl");
		s_data.m_shader_texture->bind();
		std::vector<int> texture_data(s_data.max_texture_slots);
		std::iota(std::begin(texture_data), std::end(texture_data), 0);
		s_data.m_shader_texture->upload_uniform_int_array("u_textures", texture_data.data(), texture_data.size());

		s_data.m_white_texture = Texture_2D::create(1, 1);
		uint32_t white_pixel = 0xffffffff;
		s_data.m_white_texture->set_data(&white_pixel, sizeof(white_pixel));

		s_data.m_textures[0] = s_data.m_white_texture;
	}

	void Renderer_2D::shutdown()
	{
	}

	void Renderer_2D::reset_batch()
	{
		s_data.current_quad = 0;
		s_data.current_texture_slot = 1;
	}

	void Renderer_2D::begin_scene(Ref<Camara> camara)
	{
		s_data.m_shader_texture->bind();
		s_data.m_shader_texture->upload_uniform_mat4("u_view_projection", camara->get_view_projection_matrix());

		reset_batch();
	}

	void Renderer_2D::end_scene()
	{
		s_data.m_vertex_array_plane->set_vertex_attribute_data(position_name_in_shader, &s_data.positions[0][0], s_data.current_quad * 4);
		s_data.m_vertex_array_plane->set_vertex_attribute_data(color_name_in_shader, &s_data.colors[0][0], s_data.current_quad * 4);
		s_data.m_vertex_array_plane->set_vertex_attribute_data(texture_coodinate_name_in_shader, &s_data.texcoods[0][0], s_data.current_quad * 4);
		s_data.m_vertex_array_plane->set_vertex_attribute_data(texture_index_name_in_shader, s_data.texture_index.data(), s_data.current_quad * 4);
		s_data.m_vertex_array_plane->set_vertex_attribute_data(tilling_factor_name_in_shader, s_data.tilling_factor.data(), s_data.current_quad * 4);
		flush();
	}

	void Renderer_2D::flush()
	{
		for (int i = 0; i < s_data.current_texture_slot; i++)
		{
			s_data.m_textures[i]->bind(i);
		}

		Render_Command::draw_indexed(s_data.m_vertex_array_plane, s_data.current_quad * 6);
		s_data.statistics.num_draw_calls++;
	}

	void Renderer_2D::draw_quad(param p)
	{
		if (!(s_data.current_quad < s_data.max_quads))
		{
			end_scene();
			reset_batch();
		}

		int qi = s_data.current_quad;

		constexpr std::array<float, 2> dir[] =
		{
			{ 0.f, 0.f },
			{ 1.f, 0.f },
			{ 1.f, 1.f },
			{ 0.f, 1.f }
		};

		constexpr std::array<glm::vec4, 4> ref_quad_positions
		{
			glm::vec4{-0.5f,-0.5f,0.f,1.f},
			glm::vec4{0.5f,-0.5f,0.f,1.f},
			glm::vec4{0.5f,0.5f,0.f,1.f},
			glm::vec4{-0.5f,0.5f,0.f,1.f}
		};

		float texture_index = 0.f;
		if (p.texture != nullptr)
		{
			for (int i = 1; i < s_data.current_texture_slot; i++)
			{
				if (p.texture->get_render_id() == s_data.m_textures[i]->get_render_id())
				{
					texture_index = i;
					break;
				}
			}

			if (texture_index == 0.f) //a new texture
			{
				texture_index = s_data.current_texture_slot;
				s_data.m_textures[s_data.current_texture_slot] = p.texture;
				s_data.current_texture_slot++;
			}
		}
		glm::mat4 transform =
			glm::translate(glm::mat4(1.f), glm::vec3{ p.position.x,p.position.y,0.f })
			* glm::rotate(glm::mat4(1.f), p.rotation, { 0.f,0.f,1.f })
			* glm::scale(glm::mat4(1.f), { p.size.x, p.size.y,0.f });

		for (int i = 0; i < std::size(ref_quad_positions); i++)
		{
			s_data.positions[qi * 4 + i] = transform * ref_quad_positions[i];
			s_data.texcoods[qi * 4 + i] = { dir[i][0] , dir[i][1] };
			s_data.colors[qi * 4 + i] = p.tint_color;
			s_data.texture_index[qi * 4 + i] = texture_index;
			s_data.tilling_factor[qi * 4 + i] = p.tilling_factor;
		}
		s_data.current_quad++;

		s_data.statistics.num_quad++;
	}

	void Renderer_2D::reset_statistic()
	{
		s_data.statistics = {};
	}

	Renderer_2D::Statistics Renderer_2D::get_statistic()
	{
		return s_data.statistics;
	}

	int32_t Renderer_2D::Statistics::get_vertex_num()
	{
		return num_quad * 4;
	}

	int32_t Renderer_2D::Statistics::get_triangle_num()
	{
		return num_quad * 2;
	}

}
