
#include "Shader_Test.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glog/logging.h"




Shader_Test::	Shader_Test(){
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
  
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
	LOG(ERROR)<<"glad load failed";
    }
}

void Shader_Test::SetUp(){
}

TEST_F(Shader_Test,test_read_vertex_and_fragment_shader_from_file){

    shader=new Shader("test_data/plain.vs","test_data/plain.fs");
    ASSERT_NE(shader->get_program(),0);
}
