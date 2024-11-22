#ifndef INSTANCE_H
#define INSTANCE_H

#include "config.h"

//Defining handles 
namespace vkInit
{
	vk::Instance make_instance(bool debug, const char* appName) {
		if (debug)
			std::cout << "Creating instance..." << std::endl;
		/*
		* Instance stores all per-application state info, it is a vulkan handle. (Pointer value or opaque integer value referring to an Vk object)
		* 
		* from vulkan_core.h
		* VK_DEFINE_HANDLE(VkInstance)
		* 
		* from vulkan_handles.hpp
		* class Instance {
		* ...
		* }
		*/

		/*
		* Using this method to check which version it supports. As of Vulkan 1.1
		* VkResult vkEnumerateInstanceVersion( uint32_t * pApiVersion )
		*/
		uint32_t version{ 0 };
		vkEnumerateInstanceVersion(&version);

		if (debug)
		{
			std::cout << "System can support Vulkan Variant: " << VK_API_VERSION_VARIANT(version)
				<< ", Major " << VK_API_VERSION_MAJOR(version)
				<< ", Minor " << VK_API_VERSION_MINOR(version)
				<< ", Patch " << VK_API_VERSION_PATCH(version) << "\n";
		}

		//Setting version patch to 0 for better compatibility/stability
		version &= ~(0xFFFU);

		/*
		* Fron vulkan_structs.hpp
		* VULKAN_HPP_CONSTEXPR ApplicationInfo( const char * pApplicationName_   = {},
                                                uint32_t     applicationVersion_ = {},
                                                const char * pEngineName_        = {},
                                                uint32_t     engineVersion_      = {},
                                                uint32_t     apiVersion_         = {},
                                                const void * pNext_              = nullptr )
		*/	

		vk::ApplicationInfo appInfo = vk::ApplicationInfo(appName, version, "Doing hard way", version, version);

		/*
		* Need to query which extensions glfw requires to work with vulkan.
		*/
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (debug)
		{
			std::cout << "extensions to be requested:\n";
			for (const char* name : extensions)
			{
				std::cout << "\t\"" << name << "\"\n";
			}
		}

		std::vector<const char*> layers;
		/*
		* From vulkan_structs.hpp
		*  VULKAN_HPP_CONSTEXPR InstanceCreateInfo( VULKAN_HPP_NAMESPACE::InstanceCreateFlags     flags_                   = {},
                                             const VULKAN_HPP_NAMESPACE::ApplicationInfo * pApplicationInfo_        = {},
                                             uint32_t                                      enabledLayerCount_       = {},
                                             const char * const *                          ppEnabledLayerNames_     = {},
                                             uint32_t                                      enabledExtensionCount_   = {},
                                             const char * const *                          ppEnabledExtensionNames_ = {},
                                             const void *                                  pNext_                   = nullptr )
		*/
		vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(vk::InstanceCreateFlags(), &appInfo, static_cast<uint32_t>(layers.size()), layers.data(),
			static_cast<uint32_t>(extensions.size()), extensions.data()
		);

		try {
			/*
             * Creating the instance with all the info.
             *
             * from vulkan_funcs.hpp
             * createInstance( const VULKAN_HPP_NAMESPACE::InstanceCreateInfo *  pCreateInfo,
														const VULKAN_HPP_NAMESPACE::AllocationCallbacks * pAllocator,
														VULKAN_HPP_NAMESPACE::Instance *                  pInstance,
														Dispatch const &                                  d )
             */
			return vk::createInstance(createInfo);
		}
		catch (vk::SystemError err)
		{
			if (debug)
				std::cout << "Failed to create Instance.\n" << err.what();
			return nullptr;
		}
		
	}
}

#endif