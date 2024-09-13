#pragma once

#include <string_view>
#include <vector>

#include <vulkan/vulkan.h>

class PhysicalDevice;
class Surface;
class Instance
{
public:
	Instance();

	~Instance();

	VkInstance getVkInstance() const;

	PhysicalDevice* getBestPhysicalDevice(Surface* surface);

private:
	VkInstance m_vkInstance;

	std::vector<VkExtensionProperties> m_extensionProperties;

	std::vector<VkLayerProperties> m_availableLayers;
};

