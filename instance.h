#ifndef INSTANCE_H
#define INSTANCE_H

#include "config.h"

//Defining handles 
namespace vkInit
{
	/*
	* Checks if the given extensions and layers are supported.
	*/
	bool supported(std::vector<const char*>& extensions, std::vector<const char*>& layers, bool debug)
	{
		std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();

		if (debug)
		{
			std::cout << "Device can support the following extensions:\n";
			for (vk::ExtensionProperties extensionProperty : supportedExtensions)
			{
				std::cout << '\t' << extensionProperty.extensionName << '\n';
			}
		}

		for (const char* extensionName : extensions)
		{
			bool foundExtension = false;
			for (vk::ExtensionProperties extensionProperty : supportedExtensions)
			{
				if (strcmp(extensionProperty.extensionName, extensionName) == 0)
				{
					foundExtension = true;
					if (debug)
						std::cout << "Extension " << extensionName << " is supported on this device.\n";
				}
			}

			if (!foundExtension)
			{
				if (debug)
					std::cout << "Extension " << extensionName << " is not supported on this device.\n";
				return false;
			}
		}

		std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();

		if (debug)
		{
			std::cout << "Device can support the following layers:\n";
			for (vk::LayerProperties layerProperty : supportedLayers)
			{
				std::cout << '\t' << layerProperty.layerName << '\n';
			}
		}

		for (const char* layerName : layers)
		{
			bool foundLayer = false;
			for (vk::LayerProperties layerProperty : supportedLayers)
			{
				if (strcmp(layerProperty.layerName, layerName) == 0)
				{
					foundLayer = true;
					if (debug)
						std::cout << "Layer " << layerName << " is supported on this device.\n";
				}
			}

			if (!foundLayer)
			{
				if (debug)
					std::cout << "Layer " << layerName << " is not supported on this device.\n";
				return false;
			}
		}

		return true;
	}


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
			extensions.push_back("VK_EXT_debug_utils");
		}

		if (debug)
		{
			std::cout << "extensions to be requested:\n";
			for (const char* name : extensions)
			{
				std::cout << "\t\"" << name << "\"\n";
			}
		}

		std::vector<const char*> layers;
		
		if (debug)
		{
			layers.push_back("VK_LAYER_KHRONOS_validation");
		}

		if (!supported(extensions, layers, debug))
		{
			return nullptr;
		}
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
		vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(vk::InstanceCreateFlags(), &appInfo,
			static_cast<uint32_t>(layers.size()), layers.data(),
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