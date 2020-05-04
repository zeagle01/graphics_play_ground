
#pragma once


#include <vulkan/vulkan.h>
#include <vector>
#include <optional>



struct GLFWwindow;
struct VkDebugUtilsMessengerCreateInfoEXT;


struct Queue_Family_Indices
{
	std::optional<uint32_t> graphics_family;
};

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
	void setup_debug_messenger();
	void pick_physical_device();
	void create_logic_device();

	void check_extension_support();
	bool check_validation_layer_support();

	void get_requred_extensions();




	void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& create_info);

private:
	bool is_device_suitable(VkPhysicalDevice device);
	Queue_Family_Indices find_queue_families(VkPhysicalDevice device);

private:
	const unsigned int kWidth = 800;
	const unsigned int kHeight = 600;
	GLFWwindow* m_window;
	VkInstance m_vk_instance;
	VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
	VkDevice m_device;//logic device;

	VkDebugUtilsMessengerEXT m_debug_messenger;

	std::vector<const char*> m_extensions;
	const std::vector<const char*> m_validation_layers{ "VK_LAYER_KHRONOS_validation" };
#ifdef NDEBUG
	const bool m_enable_validation_layes = false;
#else
	const bool m_enable_validation_layes = true;
#endif

};