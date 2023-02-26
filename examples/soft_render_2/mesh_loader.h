
#pragma once
#include "type_map.h"
#include "mat.h"

#include <string>
#include <functional>
#include <fstream>
#include <map>  

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

		struct VertexAttribute
		{
			std::vector<vec3>* attribute;
			int obj_order;//in face
		};

		std::vector<VertexAttribute> sort_attribute_according_to_size();
		std::vector<  std::map<int, int > > record_map_of_attribute_from_high_size_to_low_size(std::vector<VertexAttribute> size_sorted_vertex_attribute);
		void permuate_attribute_according_to_map(std::vector<VertexAttribute> size_sorted_vertex_attribute, const std::vector<  std::map<int, int > >& map_greater_size_to_less_size_attribute);
		void extend_to_gl_format();

		std::ifstream fin;

	};
}