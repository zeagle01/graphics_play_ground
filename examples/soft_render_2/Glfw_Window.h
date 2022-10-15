
#pragma once

#include "GLFW/glfw3.h"
#include <functional>

class Glfw_Window
{
public:
	GLFWwindow* create_window(int width,int height);
	void main_loop(std::function<void()>);
private:
	GLFWwindow* m_window;
};