#include "Hello_VK_Triangle.h"

#include "glfw/glfw3.h"
#include "glog/logging.h"

#include <cstring>


VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		LOG(WARNING) << "VK_ERROR_EXTENSION_NOT_PRESENT";
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
	else
	{
		LOG(WARNING) << "VK_ERROR_EXTENSION_NOT_PRESENT";
	}
}


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
	setup_debug_messenger();
	pick_physical_device();
}
void Hello_VK_Triangle::pick_physical_device()
{

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
	LOG(INFO) << "--------------";


}

bool Hello_VK_Triangle::check_validation_layer_support()
{
	uint32_t layer_count;
	vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
	std::vector<VkLayerProperties> available_layer_pros(layer_count);
	vkEnumerateInstanceLayerProperties(&layer_count, available_layer_pros.data());
	for (const auto& layer : m_validation_layers)
	{
		bool layer_found = false;
		for (const auto& available_layer_pro : available_layer_pros)
		{
			if (strcmp(layer, available_layer_pro.layerName) == 0)
			{
				layer_found = true;
				break;
			}
		}
		if (!layer_found)
		{
			if (m_enable_validation_layes)
			{
				throw std::runtime_error(std::string(layer) + " validation layer requested, but not found!");
			}
			return false;
		}

	}
	return true;

}

void Hello_VK_Triangle::get_requred_extensions()
{
	uint32_t glfw_extension_count;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
	m_extensions = std::vector<const char*>(glfwExtensions, glfwExtensions + glfw_extension_count);
	if (m_enable_validation_layes)
	{
		m_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		LOG(INFO) << VK_EXT_DEBUG_UTILS_EXTENSION_NAME << " added to required extensions!";
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
	get_requred_extensions();

	instance_create_info.enabledExtensionCount = m_extensions.size();
	instance_create_info.ppEnabledExtensionNames = m_extensions.data();

	check_validation_layer_support();


	VkDebugUtilsMessengerCreateInfoEXT debug_create_info;
	if (m_enable_validation_layes)
	{
		instance_create_info.enabledExtensionCount = m_validation_layers.size();
		instance_create_info.ppEnabledLayerNames = m_validation_layers.data();
		for (const auto& layer : m_validation_layers)
		{
			LOG(INFO) << "layer: "<<layer << " enabled";
		}

		populate_debug_messenger_create_info(debug_create_info);
		instance_create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_create_info;
	}
	else
	{
		instance_create_info.enabledLayerCount = 0;
	}

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
	if (m_enable_validation_layes)
	{
		DestroyDebugUtilsMessengerEXT(m_vk_instance, m_debug_messenger, nullptr);
	}

	vkDestroyInstance(m_vk_instance, nullptr);

	glfwDestroyWindow(m_window);

	glfwTerminate();

	LOG(INFO) << "cleanup";
}


static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
	VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
	VkDebugUtilsMessageTypeFlagsEXT message_type,
	const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
	void* p_user_data
)
{
	LOG(WARNING) << "validation layer: " << p_callback_data->pMessage;
	if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		LOG(WARNING) << "severe !";
	}
	return VK_FALSE;
}


void Hello_VK_Triangle::populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& create_info)
{
	create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	create_info.pfnUserCallback = debug_callback;
	create_info.pUserData = nullptr;
}

void Hello_VK_Triangle::setup_debug_messenger()
{
	if (!m_enable_validation_layes)
	{
		return;
	}
	VkDebugUtilsMessengerCreateInfoEXT create_info{};
	populate_debug_messenger_create_info(create_info);

	if (!CreateDebugUtilsMessengerEXT(m_vk_instance, &create_info, nullptr, &m_debug_messenger) != VK_SUCCESS) 
	{
		throw std::runtime_error("can't create debug messenger!");
	}

}













