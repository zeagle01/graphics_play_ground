#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>

#include "Shader.h"
#include "Simulator.h"

#include "json/json.h"


class Scene{
    private:
    Shader* shader;


    void make_buffers();

    int m_vbo;
    int m_ebo;
    int m_vao;

    Shader* m_shader;
	std::shared_ptr<Simulator> simulator;


    std::vector<float> m_positions;
    std::vector<int> m_indices;

	std::vector<float> m_gravity;


    public:
    
    void init(int argc,char** argv);
    void main_loop(int argc,char** argv);

    void init_from_config(const std::string& config_file);

    int get_vertex_num()const ;
    int get_triangle_num()const ;

private:
    Json::Value m_config_root;
    std::string m_config_dir;

    void build_config_root(const std::string& file);

    void config_simulator();
    void config_shader();
    void config_mesh();
    void config_gravity();

};

#endif
