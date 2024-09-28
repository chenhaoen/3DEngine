#pragma once

#include <vulkan/vulkan.h>

class CommandPool
{
public:
    CommandPool();
    ~CommandPool();

    VkCommandPool getVkCommandPool() const;
private:
    VkCommandPool m_vkCommandPool;
};
