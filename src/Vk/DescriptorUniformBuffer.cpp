#include "Vk/DescriptorUniformBuffer.h"

DecriptorUniformBuffer::DecriptorUniformBuffer(VkDescriptorBufferInfo in_bufferInfo, uint32_t in_dstBinding, uint32_t in_dstArrayElement, VkDescriptorType in_descriptorType)
:Descriptor(in_dstBinding,in_dstArrayElement,in_descriptorType)
{
    m_bufferInfo = new VkDescriptorBufferInfo(in_bufferInfo);
    m_writeDescriptorSet.pBufferInfo = m_bufferInfo;
    m_writeDescriptorSet.descriptorCount = 1;
}

DecriptorUniformBuffer::~DecriptorUniformBuffer()
{
    delete m_bufferInfo;
}