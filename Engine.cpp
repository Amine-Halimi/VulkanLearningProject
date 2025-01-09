#include "Engine.h"
#include "instance.h"
#include "Logging.h"
#include "Device.h"
//Statically linking vulkan library

using std::cout;
using std::endl;

Engine::Engine()
{
	if (debugMode)
		cout << "Making Engine running" << endl;
	create_glfw_window();
	make_instance();
	make_device();
}

Engine::~Engine()
{
	if (debugMode)
		cout << "Closing Engine" << endl;

	vulkan_instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
	/*
	* from vulkan_funcs.hpp
	* 
	* void Instance::destroy( Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator, Dispatch const & d )
	*/

	vulkan_instance.destroy();
	glfwTerminate();
}

void Engine::create_glfw_window()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, "Vulkan Project", nullptr, nullptr);

	if (window)
	{
		if (debugMode)
			cout << "Window has been created" << endl;
	}

	else
	{
		if (debugMode)
			cout << "FAILED: glfw Window could not be created" << endl;
	}
}

void Engine::make_instance()
{
	vulkan_instance = vkInit::make_instance(debugMode, "Vulkan Project");
	dldi = vk::DispatchLoaderDynamic(vulkan_instance, vkGetInstanceProcAddr);
	debugMessenger = vkInit::make_debug_messenger(vulkan_instance, dldi);
}

void Engine::make_device()
{
	physicalDevice = vkInit::choose_physical_device(vulkan_instance, debugMode);
	vkInit::findQueueFamilies(physicalDevice, debugMode);
}