
#pragma once
#include "type_map.h"
#include "mat.h"

#include <string>
#include <functional>
#include <fstream>

namespace soft_render
{
	class Mesh_Loader
	{
	public: 
		void load(std::string file);
		
	public:
		struct config
		{
			ADD_MEMBER_POINTER(positions, std::vector<vec3>);
			ADD_MEMBER_POINTER(normals, std::vector<vec3>);
			ADD_MEMBER_POINTER(texture_uv, std::vector<vec3>);
			ADD_MEMBER_POINTER(indicies, std::vector<vec3i>);
		};
		type_map m_configs;
	private:

		using obj_tri_t = std::array<vec3i, 3>;
		std::vector<obj_tri_t> m_obj_indices;

		void loop_lines(
			std::function<void(const vec3&)> pos_fn,
			std::function<void(const vec3&)> nrm_fn,
			std::function<void(const vec3&)> uv_fn,
			std::function<void(const obj_tri_t&)> tri_fn
			);

		void extend_to_gl_format();

		std::ifstream fin;

	};
}