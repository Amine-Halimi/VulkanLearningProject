#ifndef ENGINE_H
#define ENGINE_H

#include "config.h"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

class Engine {
public:
	Engine();
	~Engine();
private:

	bool debugMode = true;

	int width{ WINDOW_WIDTH };
	int height{WINDOW_HEIGHT};

	GLFWwindow* window {nullptr};

	//vulkan
	vk::Instance vulkan_instance{ nullptr };
	//debug callback
	vk::DebugUtilsMessengerEXT debugMessenger{ nullptr };
	//dispatch instance dispatcher
	vk::DispatchLoaderDynamic dldi;
	
	vk::PhysicalDevice physicalDevice{ nullptr };


	void create_glfw_window();

	void make_instance();

	void make_device();

};

#endif