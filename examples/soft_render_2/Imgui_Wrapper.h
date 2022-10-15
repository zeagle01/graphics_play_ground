
#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


struct GLFWwindow;

class Imgui_Wrapper
{
public:
	void init(GLFWwindow* window);
	void update();
private:

};
