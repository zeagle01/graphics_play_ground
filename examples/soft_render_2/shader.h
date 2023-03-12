
#pragma once

#include "type_map.h"
#include "mat.h" 

namespace soft_render
{

	class Drawing_Buffer;

	class Shader
	{
	public:

		void init(Drawing_Buffer* sc);

		void draw_triangles();
		void draw_lines();

		template<typename VarName,typename T>
		void set_vertex_attribute(const T& v)
		{
			//TODO:  restriction
			m_configs.get_ref<VarName>() = v;
		}

		template<typename VarName,typename T>
		void set_uniform(const T& v)
		{
			//TODO:  restriction
			m_configs.get_ref<VarName>() = v;
		}

	private:

		Drawing_Buffer* m_screen;

		struct fragment_in
		{
			ADD_MEMBER_POINTER(pos, vec3 );
			ADD_MEMBER_POINTER(normal, vec3 );
			ADD_MEMBER_POINTER(uv, vec3 );
		};
		type_map m_fragment_in;

	public:

		void transform_uniform();
		void compute_vertex_shader(int vi);
		vec3 compute_fragmen_shader();

		struct config
		{
			ADD_MEMBER_POINTER(pos, std::vector<vec3> );
			ADD_MEMBER_POINTER(indices, std::vector<vec3i> );
			ADD_MEMBER_POINTER(normal, std::vector<vec3> );
			ADD_MEMBER_POINTER(uv, std::vector<vec3> );

			ADD_MEMBER_POINTER(edge_indices, std::vector<vec2i> );

			ADD_MEMBER_POINTER(view_matrix, mat4 );
			ADD_MEMBER_POINTER(projection_matrix, mat4 );
			ADD_MEMBER_POINTER(model_matrix, mat4 );

			ADD_MEMBER_POINTER(light_pos, vec3 );
			ADD_MEMBER_POINTER(light_color, vec3 );
			ADD_MEMBER_POINTER(ambient, float );
			ADD_MEMBER_POINTER(obj_color, vec3 );
			ADD_MEMBER_POINTER(out_light_pos, vec3 );

			ADD_MEMBER_POINTER(out_pos, std::vector<vec3> );
			ADD_MEMBER_POINTER(out_normal, std::vector<vec3> );
			ADD_MEMBER_POINTER(out_uv, std::vector<vec3> );

		};
		type_map m_configs;

	};
}
