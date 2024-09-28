#include <stdexcept>

#include "Vk/CommandPool.h"
#include "Vk/LogicalDevice.h"
#include "Vk/PhysicalDevice.h"

#include "Base/Context.h"

CommandPool::CommandPool()
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = Context::instance()->getDevice()->getPhysicalDevice()->getGraphicsFamilyIndex();

    if (vkCreateCommandPool(Context::instance()->getDevice()->getVkDevice(), &poolInfo, nullptr, &m_vkCommandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }
}

CommandPool::~CommandPool()
{
    vkDestroyCommandPool(Context::instance()->getDevice()->getVkDevice(), m_vkCommandPool, nullptr);
}

VkCommandPool CommandPool::getVkCommandPool() const
{
    return m_vkCommandPool;
}