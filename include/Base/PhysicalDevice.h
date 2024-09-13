#pragma once

#include <vulkan/vulkan.h>

class Instance;
class Surface;
class PhysicalDevice
{
public:
	PhysicalDevice(VkPhysicalDevice vkPhysicalDevice, 
		Instance* instance,
		Surface* surface);
	~PhysicalDevice();

	VkPhysicalDevice getVkPhysicalDevice() const;

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	uint32_t getGraphicsFamilyIndex() const;
	uint32_t getPresentFamilyIndex() const;
private:
	VkPhysicalDevice m_vkPhysicalDevice;

	VkPhysicalDeviceProperties m_deviceProperties;
	VkPhysicalDeviceFeatures m_deviceFeatures;

	uint32_t m_graphicsFamilyIndex;

	uint32_t m_presentFamilyIndex;
};

