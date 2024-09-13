#include <stdexcept>
#include <iostream>

#include "Base/Instance.h"
#include "Base/DebugUtilsMessenger.h"
#include "Base/Application.h"
#include "Base/PhysicalDevice.h"

Instance::Instance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = Application::instance()->getName().data();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "3D Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    const auto& windowExtensions = 

    createInfo.enabledExtensionCount = Application::instance()->getInstanceExtensions().size();
    createInfo.ppEnabledExtensionNames = Application::instance()->getInstanceExtensions().data();

    createInfo.enabledLayerCount = Application::instance()->getInstanceLayers().size();
    createInfo.ppEnabledLayerNames = Application::instance()->getInstanceLayers().data();

    if (Application::instance()->debug())
    {
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        DebugUtilsMessenger::populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }

    if (vkCreateInstance(&createInfo, nullptr, &m_vkInstance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }

    {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        m_extensionProperties.resize(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, m_extensionProperties.data());

        if (Application::instance()->debug())
        {
            std::cout << "available instance extensions:\n";

            for (const auto& extension : m_extensionProperties) {
                std::cout << '\t' << extension.extensionName << '\n';
            }
        }
    }

    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        m_availableLayers.resize(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, m_availableLayers.data());

        if (Application::instance()->debug())
        {
            std::cout << "available instance layers:\n";

            for (const auto& layer : m_availableLayers) {
                std::cout << '\t' << layer.layerName << '\n';
            }
        }
    }
}

Instance::~Instance()
{
    vkDestroyInstance(m_vkInstance, nullptr);
}

VkInstance Instance::getVkInstance() const
{
    return m_vkInstance;
}

PhysicalDevice* Instance::getBestPhysicalDevice(Surface* surface)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, nullptr);

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, devices.data());

    if (deviceCount == 0)
    {
        return nullptr;
    }

    VkPhysicalDevice bestPhysicalDevice = devices.front();

    return new PhysicalDevice(bestPhysicalDevice,this, surface);
}
