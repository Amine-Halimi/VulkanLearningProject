#ifndef DEVICE_H
#define DEVICE_H
#include "config.h"
/*
* Chooses which physical would fit with our current specification
*/

namespace vkInit
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}

	};
	/*
	* Describes the properties of the device.
	*/
	void log_device_properties(vk::PhysicalDevice device)
	{
		vk::PhysicalDeviceProperties properties = device.getProperties();

		std::cout << "Device name " << properties.deviceName << std::endl;

		std::cout << "Device type: ";

		switch (properties.deviceType)
		{
			case (vk::PhysicalDeviceType::eCpu):
				std::cout << "CPU\n"; break;
			case (vk::PhysicalDeviceType::eDiscreteGpu):
				std::cout << "Discrete GPU\n"; break;
			case (vk::PhysicalDeviceType::eIntegratedGpu):
				std::cout << "Integrated GPU\n"; break;
			case (vk::PhysicalDeviceType::eVirtualGpu):
				std::cout << "Virtual GPU\n"; break;
			default: std::cout << "Other\n";
		}
	}

	/*
	* Checking if the device supports the requestedExtensions
	*/
	bool checkDeviceExtensionSupport(const vk::PhysicalDevice& device, const std::vector<const char*>& requestedExtensions, const bool& debug)
	{
		std::set<std::string> requiredExtensions(requestedExtensions.begin(), requestedExtensions.end());

		if (debug)
			std::cout << "Device can support the following extensions: \n";

		for (vk::ExtensionProperties& extension : device.enumerateDeviceExtensionProperties())
		{
			if (debug)
				std::cout << "\t\"" << extension.extensionName << "\"\n";

			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	/*
	* Check if the device can be used
	*/

	bool isSuitable(const vk::PhysicalDevice& device, const bool debug)
	{
		if (debug)
		{
			std::cout << "Checking if device is suitable\n";
		}

		const std::vector<const char*> requestedExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		if (debug)
		{
			std::cout << "We are requesting device extensions:\n";

			for (const char* extension : requestedExtensions)
			{
				std::cout << "\t\"" << extension << "\"\n";
			}
		}

		if (bool extensionSupported = checkDeviceExtensionSupport(device, requestedExtensions, debug))
		{
			if (debug)
				std::cout << "Device can support the requested extensions.\n";
		}
		else
		{
			if (debug)
				std::cout << "Device cannot support the requested extensions.\n";

			return false;
		}

		return true;
	}

	vk::PhysicalDevice choose_physical_device(vk::Instance& vulkan_instance, bool debug)
	{
		if (debug)
		{
			std::cout << "Choosing Physical device..." << std::endl;
		}
		/*
		*   template <typename PhysicalDeviceAllocator, typename Dispatch>
			ResultValueType<std::vector<PhysicalDevice, PhysicalDeviceAllocator>>::type
                       Instance::enumeratePhysicalDevices( Dispatch const & d ) const
		*/
		std::vector<vk::PhysicalDevice> availableDevices = vulkan_instance.enumeratePhysicalDevices();

		if (debug)
		{
			std::cout << "There are " << availableDevices.size() << " physical devices available on this system." << std::endl;
		}

		for (vk::PhysicalDevice device : availableDevices)
		{
			if (debug)
			{
				log_device_properties(device);
			}

			if (isSuitable(device, debug))
			{
				return device;
			}
		}

		return nullptr;
	}

	QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice& device, bool debug)
	{
		QueueFamilyIndices indices;

		std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

		if (debug)
		{
			std::cout << "Device can support " << queueFamilies.size() << " queue families.\n";
		}

		int i = 0;
		for (vk::QueueFamilyProperties queueFamily : queueFamilies)
		{
			/* // Provided by VK_VERSION_1_0
				typedef struct VkQueueFamilyProperties {
				VkQueueFlags    queueFlags;
				uint32_t        queueCount;
				uint32_t        timestampValidBits;
				VkExtent3D      minImageTransferGranularity;
			} VkQueueFamilyProperties;

			queueFlags is a bitmask of VkQueueFlagBits indicating capabilities of the queues in this queue family.

				queueCount is the unsigned integer count of queues in this queue family.Each queue family must support
				at least one queue.

				timestampValidBits is the unsigned integer count of meaningful bits in the timestamps written via
				vkCmdWriteTimestamp.The valid range for the count is 36..64 bits, or a value of 0,
				indicating no support for timestamps.Bits outside the valid range are guaranteed to be zeros.

				minImageTransferGranularity is the minimum granularity supported for image transfer
				operations on the queues in this queue family.
				* /

				/*
				* // Provided by VK_VERSION_1_0
					typedef enum VkQueueFlagBits {
					VK_QUEUE_GRAPHICS_BIT = 0x00000001,
					VK_QUEUE_COMPUTE_BIT = 0x00000002,
					VK_QUEUE_TRANSFER_BIT = 0x00000004,
					VK_QUEUE_SPARSE_BINDING_BIT = 0x00000008,
					} VkQueueFlagBits;
				*/

			if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				indices.graphicsFamily = i;
				indices.presentFamily = i;

				if (debug)
					std::cout << "Queue Family " << i << " is suitable for graphics and presenting.\n";
			}

			if (indices.isComplete())
			{
				break;
			}

			i++;
		}

		return indices;
	}
}

#endif
