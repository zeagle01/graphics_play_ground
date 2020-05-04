
#pragma once


#include <vulkan/vulkan.h>



struct GLFWwindow;

class Hello_VK_Triangle
{

public:
	void run();


private:
	void init_window();
	void main_loop();
	void cleanup();

	void init_vulkan();
	void craete_vk_instance();
	void check_extension_support();
private:
	const unsigned int kWidth = 800;
	const unsigned int kHeight = 600;
	GLFWwindow* m_window;
	VkInstance m_vk_instance;
};