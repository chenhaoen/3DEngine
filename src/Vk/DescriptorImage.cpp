#include "Vk/DescriptorImage.h"

DescriptorImage::DescriptorImage(VkDescriptorImageInfo in_imageInfo, uint32_t in_dstBinding, uint32_t in_dstArrayElement, VkDescriptorType in_descriptorType)
:Descriptor(in_dstBinding,in_dstArrayElement,in_descriptorType)
{
    m_imageInfo = new VkDescriptorImageInfo(in_imageInfo);
    m_writeDescriptorSet.pImageInfo = m_imageInfo;
    m_writeDescriptorSet.descriptorCount = 1;
}

DescriptorImage::~DescriptorImage()
{
    delete m_imageInfo;
}