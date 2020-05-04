
#pragma once

#include <vulkan/vulkan.h>





class Hello_VK_Triangle
{

public:
	void run();


private:
	void init_vulkan();
	void main_loop();
	void cleanup();
};