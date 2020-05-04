#include "Hello_VK_Triangle.h"

#include "glfw/glfw3.h"
#include "glog/logging.h"

#include <vector>

void Hello_VK_Triangle::run()
{
	init_window();
	init_vulkan();
	main_loop();
	cleanup();
}

void Hello_VK_Triangle::init_window()
{

	
	glfwInit();

	//not to create openGL context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_window = glfwCreateWindow(kWidth, kHeight, "vulkan window", nullptr, nullptr);

	if (m_window)
	{
		LOG(INFO) << "window " << (void*)m_window << " created";
	}
	else
	{
		LOG(ERROR) << "window faile to create" ;
		throw std::runtime_error("window faile to create");
	}


//	glm::mat4 matrix;
//	glm::vec4 vec;
//	auto test = matrix * vec;
//

}

void Hello_VK_Triangle::init_vulkan()
{
	craete_vk_instance();
}


void Hello_VK_Triangle::check_extension_support()
{
	uint32_t extension_count;
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
	std::vector<VkExtensionProperties> extensions(extension_count);

	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
	LOG(INFO) << "Vulkan extension name:";
	for (const auto &extension : extensions)
	{
		LOG(INFO) << extension.extensionName;
	}


}

void Hello_VK_Triangle::craete_vk_instance() 
{
	VkApplicationInfo app_info{};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "hello vulkan triangle";
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "No engine";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instance_create_info{};
	instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.pApplicationInfo = &app_info;

	check_extension_support();

	uint32_t glfw_extension_count;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
	instance_create_info.enabledExtensionCount = glfw_extension_count;
	instance_create_info.ppEnabledExtensionNames = glfwExtensions;
	instance_create_info.enabledLayerCount = 0;

	if (vkCreateInstance(&instance_create_info, nullptr, &m_vk_instance)!=VK_SUCCESS)
	{
		throw std::runtime_error("can't create vulkan instance");
	}
	else
	{
		LOG(INFO) << "vulkan instance " << (void*)&m_vk_instance << " created";
	}


}

void Hello_VK_Triangle::main_loop()
{

	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
	}

}
void Hello_VK_Triangle::cleanup()
{
	vkDestroyInstance(m_vk_instance, nullptr);

	glfwDestroyWindow(m_window);

	glfwTerminate();

	LOG(INFO) << "cleanup";
}
