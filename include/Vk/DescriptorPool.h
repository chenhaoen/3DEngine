#pragma once

#include <vulkan/vulkan.h>

class DescriptorPool
{
public:
    DescriptorPool();
    ~DescriptorPool();

    VkDescriptorPool getVkDescriptorPool();

private:
    VkDescriptorPool m_descriptorPool;
};