#include "Vk/Descriptor.h"

Descriptor::Descriptor(uint32_t in_dstBinding, uint32_t in_dstArrayElement, VkDescriptorType in_descriptorType)
    : m_writeDescriptorSet{}
{
    m_writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    m_writeDescriptorSet.dstBinding = in_dstBinding;
    m_writeDescriptorSet.dstArrayElement == in_dstArrayElement;
    m_writeDescriptorSet.descriptorType = in_descriptorType;
}

Descriptor::~Descriptor()
{
}

void Descriptor::bindSet(VkDescriptorSet dstSet)
{
    m_writeDescriptorSet.dstSet = dstSet;
}

VkWriteDescriptorSet Descriptor::getWriteDescriptorSet()
{
    return m_writeDescriptorSet;
}