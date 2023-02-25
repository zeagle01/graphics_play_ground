
#include "shader.h"
#include "Drawing_Buffer.h"
#include "graphic_math.h"

#include <algorithm>

namespace soft_render
{


	void Shader::init(Drawing_Buffer* sc)
	{
		m_screen = sc;
		type_map::fill_types<config>(m_configs);
		type_map::fill_types<fragment_in>(m_fragment_in);
	}

	void Shader::commit_draw()
	{
		const auto& positions = m_configs.get_ref<config::pos>();

		auto& out_positions = m_configs.get_ref<config::out_pos>();
		auto& out_normal = m_configs.get_ref<config::out_normal>();
		auto& out_uv = m_configs.get_ref<config::out_uv>();
		out_positions.resize(positions.size());
		out_normal.resize(positions.size());
		out_uv.resize(positions.size());

		for (int vi = 0; vi < positions.size(); vi++)
		{
			compute_vertex_shader(vi);
		}

		transform_uniform();
		//

		const auto& indices = m_configs.get_ref<config::indices>();
		for (int ti = 0; ti < indices.size(); ti++)
		{
			std::array<vec3, 3> p{ out_positions[indices[ti](0)],out_positions[indices[ti](1)],out_positions[indices[ti](2)] };
			std::array<vec3, 3> n{ out_normal[indices[ti](0)],out_normal[indices[ti](1)],out_normal[indices[ti](2)] };

			auto fragment_shader_fn = [this, p, n](vec3 w)
			{
				auto& fragment_p = m_fragment_in.get_ref<fragment_in::pos >();
				auto& fragment_n = m_fragment_in.get_ref<fragment_in::normal >();
				fragment_p = w(0) * p[0] + w(1) * p[1] + w(2) * p[2];
				fragment_n = w(0) * n[0] + w(1) * n[1] + w(2) * n[2];

				//return w;
				return compute_fragmen_shader();
			};
			m_screen->draw_triangle({ p[0], p[1], p[2] }, fragment_shader_fn);
		}
	}


	void Shader::compute_vertex_shader(int vi)
	{
		const auto& pos = m_configs.get_ref<config::pos>()[vi];
		const auto& normal = m_configs.get_ref<config::normal>()[vi];
		//const auto& uv = m_configs.get_ref<config::uv>()[vi];

		const auto& M = m_configs.get_ref<config::model_matrix>();
		const auto& V = m_configs.get_ref<config::view_matrix>();
		const auto& P = m_configs.get_ref<config::projection_matrix>();

		auto& out_pos = m_configs.get_ref<config::out_pos>()[vi];
		auto& out_normal = m_configs.get_ref<config::out_normal>()[vi];

		out_pos = from_homogeneous_point(P * V * M * get_homogeneous_point(pos));
		out_normal = normalize(from_homogeneous_vector(V * M * get_homogeneous_vector(normal)));

	}

	void Shader::transform_uniform()
	{

		const auto& V = m_configs.get_ref<config::view_matrix>();

		const auto& light_pos = m_configs.get_ref<config::light_pos>();
		auto& out_light_pos = m_configs.get_ref<config::out_light_pos>();

		out_light_pos = from_homogeneous_point(V * get_homogeneous_point(light_pos));
	}

	vec3 Shader::compute_fragmen_shader()
	{
		const auto& l = m_configs.get_ref<config::out_light_pos >();
		const auto& p = m_fragment_in.get_ref<fragment_in::pos >();
		const auto& n = m_fragment_in.get_ref<fragment_in::normal >();
		const auto& light_color = m_configs.get_ref<config::light_color >();
		float ambient = m_configs.get_ref<config::ambient>();
		const auto& obj_color = m_configs.get_ref<config::obj_color >();

		auto e = normalize(-p);

		auto l_dir = normalize(l - p);
		float diffuse = std::max(dot(n, l_dir), 0.f);

		auto h = normalize((l_dir + e));

		float specular = std::max(std::pow(dot(h, n), 3e2f), 0.f);

		float density = std::clamp(0.5f * diffuse + 0.5f * specular, 0.00f, 0.99f);

		auto out_color = ambient * light_color + density * (1.f - ambient) * vec3 { obj_color(0)* light_color(0), obj_color(1)* light_color(1), obj_color(2)* light_color(2) };

		return out_color;
		//return   0.5f + 0.5f * n;

	}
}

