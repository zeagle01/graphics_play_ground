
#include "mesh_loader.h"
#include <sstream>
#include <algorithm>

namespace soft_render
{
	void Mesh_Loader::load(std::string file)
	{
		type_map::fill_types<config>(m_configs);
		m_obj_indices.resize(0);

		fin.open(file);

		if (fin.is_open())
		{
			auto& positions = m_configs.get_ref<config::positions>();
			auto& normals = m_configs.get_ref<config::normals>();
			auto& textur_uv = m_configs.get_ref<config::texture_uv>();
			auto& indicies = m_configs.get_ref<config::indicies>();

			loop_lines(
				[&positions](const auto& v) {positions.push_back(v); },
				[&normals](const auto& v) {normals.push_back(v); },
				[&textur_uv](const auto& v) {textur_uv.push_back(v); },
				[this](const auto& v) { m_obj_indices.push_back(v); }
			);

			printf(" done %d %d %d \n", positions.size(), normals.size(), textur_uv.size());

			extend_to_gl_format();

			printf(" done %d %d %d \n", positions.size(), normals.size(), textur_uv.size());
		}
		else
		{
			printf("open file %s failed !\n", file.c_str());
		}
	}



	std::vector<Mesh_Loader::VertexAttribute> Mesh_Loader::sort_attribute_according_to_size()
	{
		auto& positions = m_configs.get_ref<config::positions>();
		auto& textur_uv = m_configs.get_ref<config::texture_uv>();
		auto& normals = m_configs.get_ref<config::normals>();

		std::vector<vec3>* attributes[] = { &positions,&textur_uv,&normals };
		std::vector<VertexAttribute> vertex_attributes;
		for (int i = 0; i < 3; i++)
		{
			if (!attributes[i]->empty())
			{
				vertex_attributes.push_back({ attributes[i], i });
			}
		}

		std::sort
		(
			std::begin(vertex_attributes), std::end(vertex_attributes),
			[](const auto& a, const auto& b)
			{
				if (b.obj_order == 0 && a.attribute->size() == b.attribute->size())
				{
					return true;
				}
				else
				{
					return a.attribute->size() < b.attribute->size();
				}
			}
		);
		return vertex_attributes;
	}

	std::vector<  std::map<int, int > > Mesh_Loader::record_map_of_attribute_from_high_size_to_low_size(std::vector<VertexAttribute> size_sorted_vertex_attribute)
	{

		auto& indicies = m_configs.get_ref<config::indicies>();
		std::vector<  std::map<int, int > > map_greater_size_to_less_size_attribute(size_sorted_vertex_attribute.size() - 1);
		for (int ti = 0; ti < m_obj_indices.size(); ti++)
		{
			int high_size_rank = size_sorted_vertex_attribute.back().obj_order;
			vec3i high_rank_triangle;
			for (int tvi = 0; tvi < 3; tvi++)
			{
				vec3i one_vertex_index = m_obj_indices[ti][tvi];
				high_rank_triangle(tvi) = one_vertex_index(high_size_rank);
				for (int ri = 0; ri < size_sorted_vertex_attribute.size() - 1; ri++)
				{
					int low_size_rank = size_sorted_vertex_attribute[ri].obj_order;
					auto  high_size_attribute = one_vertex_index(high_size_rank);
					auto  low_size_attribute = one_vertex_index(low_size_rank);
					map_greater_size_to_less_size_attribute[ri][high_size_attribute] = low_size_attribute;
				}
			}
			indicies.push_back(high_rank_triangle);//base on hight rank attribute to form triangle
		}

		return map_greater_size_to_less_size_attribute;
	}


	void Mesh_Loader::permuate_attribute_according_to_map(std::vector<VertexAttribute> size_sorted_vertex_attribute, const std::vector<  std::map<int, int > >& map_greater_size_to_less_size_attribute)
	{
		int max_size = size_sorted_vertex_attribute.back().attribute->size();
		for (int ri = 0; ri < size_sorted_vertex_attribute.size() - 1; ri++)
		{
			int low_size_rank = size_sorted_vertex_attribute[ri].obj_order;
			auto low_size_attribute_backup = *size_sorted_vertex_attribute[ri].attribute;
			auto& low_size_attribute = *(size_sorted_vertex_attribute[ri].attribute);
			low_size_attribute.resize(max_size);
			for (int vi = 0; vi < max_size; vi++)
			{
				int maped_v = map_greater_size_to_less_size_attribute[ri].at(vi);
				low_size_attribute[vi] = low_size_attribute_backup[maped_v];
			}
		}

	}

	void Mesh_Loader::extend_to_gl_format()
	{
		auto size_sorted_vertex_attribute = sort_attribute_according_to_size();

		auto map_greater_size_to_less_size_attribute = record_map_of_attribute_from_high_size_to_low_size(size_sorted_vertex_attribute);

		permuate_attribute_according_to_map(size_sorted_vertex_attribute, map_greater_size_to_less_size_attribute);


	}

	void Mesh_Loader::loop_lines(
			std::function<void(const vec3&)> pos_fn,
			std::function<void(const vec3&)> nrm_fn,
			std::function<void(const vec3&)> uv_fn,
			std::function<void(const obj_tri_t&)> tri_fn
	)
	{

		std::string line;
		while (getline(fin, line)) {
			std::stringstream words;
			words << line;
			std::string flag;
			words >> flag;
			if (flag == "v") 
			{
				vec3 pos_i;
				for (int i = 0; i < 3; i++) {
					float v;
					words >> v;
					pos_i(i) = v;
				}
				pos_fn(pos_i);
			}
			else if (flag == "vn") 
			{
				vec3 nrm_i;
				for (int i = 0; i < 3; i++) {
					float v;
					words >> v;
					nrm_i(i) = v;
				}
				nrm_fn(nrm_i);

			}
			else if (flag == "vt") 
			{
				vec3 uv_i;
				for (int i = 0; i < 3; i++) 
				{
					float v;
					words >> v;
					uv_i(i) = v;
				}
				uv_fn(uv_i);

			}
			else if (flag == "f") {

				obj_tri_t three_vertex_indices;
				for (int i = 0; i < 3; i++) 
				{
					std::string vertex;
					words >> vertex;
					std::stringstream vertex_ss;
					vertex_ss << vertex;
					std::string vv;
					vec3i v_indices{ -1,-1,-1 };
					int vnt_i = 0;
					while (getline(vertex_ss, vv, '/'))
					{
						std::stringstream vv_ss;
						vv_ss << vv;
						int v = -1;
						vv_ss >> v;
						v -= 1;

						v_indices(vnt_i) = v;
						vnt_i++;
					}
					three_vertex_indices[i] = v_indices;
				}
				tri_fn(three_vertex_indices);
			}
		}

	}

}

