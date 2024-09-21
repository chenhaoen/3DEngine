#include <iostream>

#include "Base/PhysicalDevice.h"
#include "Base/Instance.h"
#include "Base/Application.h"
#include "Base/Surface.h"

PhysicalDevice::PhysicalDevice(VkPhysicalDevice vkPhysicalDevice,
	Instance* instance,
	Surface* surface)
	:m_vkPhysicalDevice(vkPhysicalDevice)
	, m_graphicsFamilyIndex(-1)
	, m_presentFamilyIndex(-1)
{
	vkGetPhysicalDeviceProperties(m_vkPhysicalDevice, &m_deviceProperties);
	vkGetPhysicalDeviceFeatures(m_vkPhysicalDevice, &m_deviceFeatures);
	 
	if (Application::instance()->debug())
	{
		std::cout << "Device Name: " << m_deviceProperties.deviceName << std::endl;

		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(m_vkPhysicalDevice, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(m_vkPhysicalDevice, nullptr, &extensionCount, availableExtensions.data());

		std::cout << "available device extensions:\n";

		for (const auto& extension : availableExtensions) {
			std::cout << '\t' << extension.extensionName << '\n';
		}
	}

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {

		if (m_graphicsFamilyIndex == -1)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				m_graphicsFamilyIndex = i;
			}
		}

		if (m_presentFamilyIndex == -1)
		{
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_vkPhysicalDevice, i, surface->getVkSurface(), &presentSupport);

			if (presentSupport) {
				m_presentFamilyIndex = i;
			}
		}

		i++;
	}
}

PhysicalDevice::~PhysicalDevice()
{
}

VkPhysicalDevice PhysicalDevice::getVkPhysicalDevice() const
{
	return m_vkPhysicalDevice;
}

uint32_t PhysicalDevice::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(m_vkPhysicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

uint32_t PhysicalDevice::getGraphicsFamilyIndex() const
{
	return m_graphicsFamilyIndex;
}

uint32_t PhysicalDevice::getPresentFamilyIndex() const
{
	return m_presentFamilyIndex;
}
