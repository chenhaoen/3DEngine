#pragma once

#include "Vk/Descriptor.h"

class DescriptorImage : public Descriptor
{
public:
    DescriptorImage(VkDescriptorImageInfo in_imageInfo, uint32_t in_dstBinding, uint32_t in_dstArrayElement, VkDescriptorType in_descriptorType);
    ~DescriptorImage() override; 

private:
    VkDescriptorImageInfo *m_imageInfo;
};