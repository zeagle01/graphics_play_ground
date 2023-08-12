
#include "Scene.h"
#include "Shader.h"
#include "Mesh_Loader.h"
#include "PD_Simulator.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glog/logging.h"

#include <fstream>

#include <iostream>
//#include <boost/filesystem.hpp>
#include <filesystem>


int Scene::get_vertex_num()const {
    return m_positions.size()/3;
}
int Scene::get_triangle_num()const {
    return m_indices.size()/3;
}


void Scene::make_buffers(){

    glGenVertexArrays(1,(GLuint*)&m_vao);
    glGenBuffers(1,(GLuint*)&m_vbo);
    glGenBuffers(1,(GLuint*)&m_ebo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*m_positions.size(),m_positions.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(int)*m_indices.size(),m_indices.data(),GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

}

void Scene::init(int argc,char** argv){
    FLAGS_log_dir=".";
    google::InitGoogleLogging(argv[0]);

    m_shader=new Shader("vertex.vs","fragment.frag");

}

void Scene::config_simulator()
{
    auto solver_type_node = m_config_root["simulator"];

    simulator = Simulator::new_instance(solver_type_node.asString());
    if(simulator)
    {
        LOG(INFO)<<"simulator created: "<<solver_type_node.asString();
    }
    else
    {
        LOG(ERROR)<<"simulator created failed! ";
    }

}

void Scene::config_shader()
{

    auto vertex_shader_file = m_config_dir + m_config_root["vertex_shader"].asString();
    auto fragment_shader_file = m_config_dir + m_config_root["fragment_shader"].asString();
    m_shader = new Shader(vertex_shader_file, fragment_shader_file);
}

void Scene::config_mesh()
{

    Mesh_Loader mesh_loader;
    mesh_loader.load_from_obj(m_config_dir+m_config_root["mesh_file"].asString());
    m_positions=mesh_loader.get_positions();
    m_indices=mesh_loader.get_indices();
}

void Scene::build_config_root(const std::string& file)
{
    //std::string current_config_path = boost::filesystem::system_complete(config_file).c_str();
    std::ifstream fin(file);
	if (fin.good()) {
		LOG(INFO) << "config file: " << file << "opend!";
		fin >> m_config_root;
        m_config_dir = std::filesystem::absolute(file).parent_path().string() + "/";
    }
	else {
		LOG(ERROR) << "can't open config file" << file;
	}
}

void Scene::config_gravity()
{
	Json::Value gravity_node= m_config_root["gravity_acceleration"];
	for (int i = 0; i < gravity_node.size(); i++) {
		m_gravity.push_back(gravity_node[i].asFloat());
	}
}

void Scene::init_from_config(const std::string& config_file){

    build_config_root(config_file);

    config_shader();

    //mesh
    config_mesh();

    config_simulator();


	//
	//gravity
    config_gravity();


    simulator->setMesh(m_positions, m_indices);

    simulator->setGravity(m_gravity);
	//mass
	simulator->setMass(m_config_root["uniform_mass"].asFloat());
	//dt
	simulator->setDeltaT(m_config_root["delta_t"].asFloat());

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    DLOG(INFO)<<key;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void Scene::main_loop(int argc,char** argv){

    LOG(INFO)<<"into  main loop";


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);

#endif

    GLFWwindow* window=glfwCreateWindow(800,600,"hi graphic world",NULL,NULL); 

    if(window==nullptr){
		LOG(ERROR)<<"window create failed";
	}
	else {
		LOG(INFO)<<"window create succeed!";
	}

    glfwMakeContextCurrent(window);


    glfwSetKeyCallback(window,key_callback);


    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		LOG(ERROR)<<"glad load failed";
	}
	else {
		LOG(INFO)<<"glad load succeed!";
	}

	if (argv[1]) {
		LOG(INFO) << "init from config file: " << argv[1]  ;
		init_from_config(argv[1]);
	}
	else {
		LOG(FATAL) << "specify a scene config file first!";
	}


    make_buffers();

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    while(!glfwWindowShouldClose(window)){


        glClearColor(0.2,0.3,0.1,1);
        glClear(GL_COLOR_BUFFER_BIT);

		simulator->update(m_positions,  m_indices );

        m_shader->use();
        glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
		glBufferData(GL_ARRAY_BUFFER,sizeof(float)*m_positions.size(),m_positions.data(),GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(int)*m_indices.size(),m_indices.data(),GL_STATIC_DRAW);

        glDrawElements(GL_TRIANGLES,m_indices.size(),GL_UNSIGNED_INT,0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

}
