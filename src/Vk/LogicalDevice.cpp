#include <stdexcept>
#include <set>

#include "Vk/LogicalDevice.h"
#include "Vk/PhysicalDevice.h"

#include "Base/Application.h"
#include "Base/Context.h"

LogicalDevice::LogicalDevice(PhysicalDevice* physicalDevice)
	:m_physicalDevice(physicalDevice)
{
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { m_physicalDevice->getGraphicsFamilyIndex(),
		m_physicalDevice->getPresentFamilyIndex() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = queueCreateInfos.size();

	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = true;
	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = Context::instance()->getDeviceExtensions().size();
	createInfo.ppEnabledExtensionNames = Context::instance()->getDeviceExtensions().data();

	if (Application::instance()->debug()) {
		createInfo.enabledLayerCount = Context::instance()->getInstanceLayers().size();
		createInfo.ppEnabledLayerNames = Context::instance()->getInstanceLayers().data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(m_physicalDevice->getVkPhysicalDevice(), &createInfo, nullptr, &m_vkDevice) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(m_vkDevice, m_physicalDevice->getGraphicsFamilyIndex(), 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_vkDevice, m_physicalDevice->getPresentFamilyIndex(), 0, &m_presentQueue);
}

LogicalDevice::~LogicalDevice()
{
	vkDestroyDevice(m_vkDevice, nullptr);
}

VkDevice LogicalDevice::getVkDevice() const
{
	return m_vkDevice;
}

PhysicalDevice* LogicalDevice::getPhysicalDevice() const
{
	return m_physicalDevice;
}

VkQueue LogicalDevice::getGraphicsQueue() const
{
	return m_graphicsQueue;
}

VkQueue LogicalDevice::getPresentQueue() const
{
	return m_presentQueue;
}

void LogicalDevice::Wait() const
{
	vkDeviceWaitIdle(m_vkDevice);
}
