 #include <stdexcept>
 
 #include "Vk/DescriptorPool.h"
 #include "Vk/LogicalDevice.h"

 #include "Base/Context.h"
 #include "Base/Application.h"
 
DescriptorPool::DescriptorPool()
{
 VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(Application::maxFrameCount());

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;

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
