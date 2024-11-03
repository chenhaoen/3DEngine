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

VkCommandBuffer CommandPool::createCommands()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_vkCommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(Context::instance()->getDevice()->getVkDevice(), &allocInfo, &commandBuffer);

    return commandBuffer;
}

void CommandPool::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(Context::instance()->getDevice()->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(Context::instance()->getDevice()->getGraphicsQueue());

    vkFreeCommandBuffers(Context::instance()->getDevice()->getVkDevice(), m_vkCommandPool, 1, &commandBuffer);
}