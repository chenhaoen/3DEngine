#pragma once

#include <vulkan/vulkan.h>

class PhysicalDevice;
class Node;
class LogicalDevice
{
public:
	LogicalDevice(PhysicalDevice* physicalDevice);
	~LogicalDevice();

	VkDevice getVkDevice() const;
	PhysicalDevice* getPhysicalDevice() const;

	VkQueue getGraphicsQueue() const;
	VkQueue getPresentQueue() const;

	void Wait() const;
private:
	VkDevice m_vkDevice;

	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;

	PhysicalDevice* m_physicalDevice;
};

