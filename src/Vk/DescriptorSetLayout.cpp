#include <stdexcept>

#include "Vk/DescriptorSetLayout.h"
#include "Vk/LogicalDevice.h"

#include "Base/Context.h"

DescriptorSetLayout::DescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(Context::instance()->getDevice()->getVkDevice(), &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

DescriptorSetLayout::~DescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(Context::instance()->getDevice()->getVkDevice(), m_descriptorSetLayout, nullptr);
}

VkDescriptorSetLayout DescriptorSetLayout::getVkDescriptorSetLayout() const
{
    return m_descriptorSetLayout;
}