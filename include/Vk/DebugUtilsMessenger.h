#pragma once

#include <vulkan/vulkan.h>

class Instance;

class DebugUtilsMessenger
{
public:
	DebugUtilsMessenger(Instance* instance);
	~DebugUtilsMessenger();

	static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

private:
	VkDebugUtilsMessengerEXT m_vkDebugUtilsMessenger;

	Instance* m_instance;
};

