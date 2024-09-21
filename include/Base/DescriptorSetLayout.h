#pragma once

#include <vulkan/vulkan.h>

class LogicalDevice;
class DescriptorSetLayout
{
public:
    DescriptorSetLayout(LogicalDevice *device);
    ~DescriptorSetLayout();

    VkDescriptorSetLayout getVkDescriptorSetLayout() const;

private:
    LogicalDevice* m_device;
    VkDescriptorSetLayout m_descriptorSetLayout;
};