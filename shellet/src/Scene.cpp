
#include "Scene.h"
#include "Shader.h"
#include "Mesh_Loader.h"
#include "PD_Simulator.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glog/logging.h"

#include "json/json.h"
#include <fstream>

#include <iostream>
#include <boost/filesystem.hpp>

#include "camara.h"
#include "predef_types.h"


std::shared_ptr<Scene> Scene::singleton = nullptr;
std::shared_ptr<Scene> Scene::getSingleton() {

	if (!singleton) {
		singleton = std::make_shared<Scene>();
	}
	return singleton;


}

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

std::shared_ptr<camara_np::Camara> Scene::getCamara() {
	return camara;
}

void Scene::init(int argc,char** argv){
    FLAGS_log_dir=".";
    google::InitGoogleLogging(argv[0]);

    //m_shader=new Shader("vertex.vs","fragment.frag");

}


void Scene::init_from_config(const std::string& config_file){
    Json::Value root;
    std::string current_config_path = config_file;
    //std::string current_config_path = boost::filesystem::system_complete(config_file).c_str();
    std::ifstream fin(current_config_path);
	if (fin.good()) {
		LOG(INFO) << "config file: " << config_file << "opend!";
		fin >> root;
	}
	else {
		LOG(ERROR) << "can't open config file" << config_file;
	}

    std::string current_config_dir=boost::filesystem::system_complete(config_file).parent_path().string()+"/"; 
    m_mesh_file=current_config_dir+root["mesh_file"].asString();
    m_vertex_shader_file=current_config_dir+root["vertex_shader"].asString();
    m_fragment_shader_file=current_config_dir+root["fragment_shader"].asString();

	m_shader = std::make_shared<Shader>(m_vertex_shader_file, m_fragment_shader_file);
	camara = std::make_shared<camara_np::Walking_Camara>();

    Mesh_Loader mesh_loader;
    mesh_loader.load_from_obj(m_mesh_file);
    m_positions=mesh_loader.get_positions();
    m_indices=mesh_loader.get_indices();


	//
	simulator = std::make_shared<PD_Simulator>();
	//mesh
	simulator->setMesh(m_positions, m_indices);
	//gravity
	Json::Value gravity_node= root["gravity_acceleration"];
	std::vector<float> gravity_values;
	for (int i = 0; i < gravity_node.size(); i++) {
		gravity_values.push_back(gravity_node[i].asFloat());
	}
	simulator->setGravity(gravity_values);
	//mass
	simulator->setMass(root["uniform_mass"].asFloat());
	//dt
	simulator->setDeltaT(root["delta_t"].asFloat());

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    LOG(INFO)<<key;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	auto scene=Scene::getSingleton();
	auto camara = scene->getCamara();
	camara->on_mouse_move(xpos, ypos);

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	
	auto scene=Scene::getSingleton();
	auto camara = scene->getCamara();
	camara->mouse_click_handle(button, action, 0, 0);
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

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    while(!glfwWindowShouldClose(window)){


        glClearColor(0.2,0.3,0.1,1);
        glClear(GL_COLOR_BUFFER_BIT);

		simulator->update(m_positions,  m_indices );


		auto cam_pos = camara->cam_pos;
		auto cam_front = camara->cam_front;
		auto cam_up = camara->cam_up;

        m_shader->use();

		GLuint var_loc;
		m4f model;
		model = m4f::Identity()*0.1;
		var_loc = m_shader->get_uniform_variable("model");
		glUniformMatrix4fv(var_loc, 1, GL_FALSE, model.data());

		m4f view;
//		view<< 1, 0, 0, 0.5,
//				0, 1, 0, 0.5,
//				0, 0, 1, 0,
//				0, 0, 0, 1 ;

		view = m4f::Identity();
		view.block<3, 1>(0, 0)=((cam_front-cam_pos).cross(cam_up-cam_pos)).normalized();
		view.block<3, 1>(0, 1)=(cam_front-cam_pos).normalized();
		view.block<3, 1>(0, 2)=(cam_up-cam_pos).normalized();
		var_loc = m_shader->get_uniform_variable("view");
		glUniformMatrix4fv(var_loc, 1, GL_FALSE, view.data());

		m4f projection=m4f::Identity();
		//projection = glm::perspective(45.0f, GLfloat(Width) / GLfloat(Height), 0.1f, 100.0f);
		var_loc = m_shader->get_uniform_variable("projection");
		glUniformMatrix4fv(var_loc, 1, GL_FALSE, projection.data());

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
