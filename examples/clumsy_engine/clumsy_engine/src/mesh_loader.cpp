
#include "Mesh_Loader.h"


#include <fstream>
#include <sstream>

void Mesh_Loader::load_from_obj(std::string obj_file){
    std::ifstream fin(obj_file);
	if (!fin.good()) {
		std::stringstream ss;
		ss << "can't load obj from " << obj_file;
	}
    std::string line;
	while (getline(fin, line)) {
		std::stringstream words;
		words << line;
		std::string flag;
		words >> flag;
		if (flag == "v") {
			for (int i = 0; i < 3; i++) {
				float v;
				words >> v;
				m_positions.push_back(v);
			}
		}
		else if (flag == "f") {
			
			for (int i = 0; i < 3; i++) {
				std::string vertex;
				words >> vertex;
				std::stringstream vertex_ss;
				vertex_ss << vertex;
				std::string vv;
				std::vector<int> v_indices;
				while (getline(vertex_ss, vv, '/'))
				{
					std::stringstream vv_ss;
					vv_ss << vv;
					int v;
					vv_ss >> v;
					v -= 1;
					v_indices.push_back(v);
				}
				m_indices.push_back(v_indices[0]);
			}
		}
	}

}
std::vector<int> Mesh_Loader::get_indices(){
    return m_indices;
}
std::vector<float> Mesh_Loader::get_positions(){
    return m_positions;
}
bool Mesh_Loader::is_texture_loaded(){
    return false;
}
