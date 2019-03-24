#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include <string>
#include <vector>
class Mesh_Loader{
    public:
	void load_from_obj(std::string obj_file);
	std::vector<int> get_indices();
	std::vector<float> get_positions();
	bool is_texture_loaded();

    private:
	std::vector<int> m_indices;
	std::vector<float> m_positions;
};

#endif
