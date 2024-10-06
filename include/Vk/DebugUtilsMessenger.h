#pragma once

#include <vulkan/vulkan.h>

class DebugUtilsMessenger
{
public:
	DebugUtilsMessenger();
	~DebugUtilsMessenger();

	static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

private:
	VkDebugUtilsMessengerEXT m_vkDebugUtilsMessenger;
};

