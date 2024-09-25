#pragma once

#include <vulkan/vulkan.h>

class LogicalDevice;
class CommandPool
{
public:
    CommandPool(LogicalDevice* device);
    ~CommandPool();

    VkCommandPool getVkCommandPool() const;
private:
    VkCommandPool m_vkCommandPool;

    LogicalDevice* m_device;
};
