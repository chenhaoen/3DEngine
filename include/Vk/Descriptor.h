#pragma once

#include <vulkan/vulkan.h>

class Descriptor
{
public:
    Descriptor(uint32_t in_dstBinding, uint32_t in_dstArrayElement, VkDescriptorType in_descriptorType);
    virtual ~Descriptor();

    void bindSet(VkDescriptorSet);

    VkWriteDescriptorSet getWriteDescriptorSet();

protected:
    VkWriteDescriptorSet m_writeDescriptorSet;
};