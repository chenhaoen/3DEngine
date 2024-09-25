#include <stdexcept>

#include "Base/CommandPool.h"
#include "Base/LogicalDevice.h"
#include "Base/PhysicalDevice.h"

CommandPool::CommandPool(LogicalDevice *device)
    : m_device(device)
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = m_device->getPhysicalDevice()->getGraphicsFamilyIndex();

    if (vkCreateCommandPool(m_device->getVkDevice(), &poolInfo, nullptr, &m_vkCommandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }
}

CommandPool::~CommandPool()
{
    vkDestroyCommandPool(m_device->getVkDevice(), m_vkCommandPool, nullptr);
}

VkCommandPool CommandPool::getVkCommandPool() const
{
    return m_vkCommandPool;
}