#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>

#include "Shader.h"
#include "Simulator.h"

#include "camara.h"

class Scene{
    private:


    void make_buffers();

    int m_vbo;
    int m_ebo;
    int m_vao;

    std::shared_ptr<Shader> m_shader;
	std::shared_ptr<Simulator> simulator;

	std::shared_ptr<camara_np::Camara> camara; 


    std::vector<float> m_positions;
    std::vector<int> m_indices;

    std::string m_mesh_file;
    std::string m_vertex_shader_file;
    std::string m_fragment_shader_file;;

    public:
    
    void init(int argc,char** argv);
    void main_loop(int argc,char** argv);

    void init_from_config(const std::string& config_file);

    int get_vertex_num()const ;
    int get_triangle_num()const ;

	std::shared_ptr<camara_np::Camara> getCamara();
	static std::shared_ptr<Scene> getSingleton();

private:
	static std::shared_ptr<Scene> singleton;

};

#endif
