#pragma once

#include "Vk/Descriptor.h"

class DecriptorUniformBuffer : public Descriptor
{
    public:
    DecriptorUniformBuffer(VkDescriptorBufferInfo in_bufferInfo, uint32_t in_dstBinding, uint32_t in_dstArrayElement, VkDescriptorType in_descriptorType);
    ~DecriptorUniformBuffer() override;

    private:
    VkDescriptorBufferInfo* m_bufferInfo;
};