#include <stdexcept>
#include <array>

#include "Vk/DescriptorPool.h"
#include "Vk/LogicalDevice.h"

#include "Base/Context.h"
#include "Base/Application.h"

DescriptorPool::DescriptorPool()
{
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(Application::maxFrameCount());
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(Application::maxFrameCount());

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = poolSizes.size();
    poolInfo.pPoolSizes = poolSizes.data();

    poolInfo.maxSets = static_cast<uint32_t>(Application::maxFrameCount());

    if (vkCreateDescriptorPool(Context::instance()->getDevice()->getVkDevice(), &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

DescriptorPool::~DescriptorPool()
{
    vkDestroyDescriptorPool(Context::instance()->getDevice()->getVkDevice(), m_descriptorPool, nullptr);
}

VkDescriptorPool DescriptorPool::getVkDescriptorPool()
{
    return m_descriptorPool;
}
