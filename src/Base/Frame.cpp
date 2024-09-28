#include <stdexcept>

#include "Base/Frame.h"
#include "Base/LogicalDevice.h"
#include "Base/CommandPool.h"
#include "Base/SwapChain.h"
#include "Base/RenderPass.h"
#include "Base/Context.h"

Frame::Frame(CommandPool *commandPool)
    : m_commandPool(commandPool)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_commandPool->getVkCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkAllocateCommandBuffers(Context::instance()->getDevice()->getVkDevice(), &allocInfo, &m_commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    if (vkCreateSemaphore(Context::instance()->getDevice()->getVkDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(Context::instance()->getDevice()->getVkDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS ||
        vkCreateFence(Context::instance()->getDevice()->getVkDevice(), &fenceInfo, nullptr, &m_inFlightFence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create semaphores!");
    }

    m_clearValue.color = {0.0f, 0.0f, 0.0f, 1.0f};
}

Frame::~Frame()
{
    vkDestroySemaphore(Context::instance()->getDevice()->getVkDevice(), m_imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(Context::instance()->getDevice()->getVkDevice(), m_renderFinishedSemaphore, nullptr);
    vkDestroyFence(Context::instance()->getDevice()->getVkDevice(), m_inFlightFence, nullptr);
}

VkCommandBuffer Frame::getCommandBuffer() const
{
    return m_commandBuffer;
}

void Frame::begin()
{
    vkWaitForFences(Context::instance()->getDevice()->getVkDevice(), 1, &m_inFlightFence, VK_TRUE, UINT64_MAX);

    vkResetFences(Context::instance()->getDevice()->getVkDevice(), 1, &m_inFlightFence);

    m_imageIndex = Context::instance()->getSwapChain()->getNextImageIndex(m_imageAvailableSemaphore);

    vkResetCommandBuffer(m_commandBuffer, 0);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;                  // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(m_commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkExtent2D swapChainExtent = Context::instance()->getSwapChain()->getExtent();

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = Context::instance()->getRenderPass()->getVkRenderPass();
    renderPassInfo.framebuffer = Context::instance()->getSwapChain()->getFrameBuffer(m_imageIndex);

    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &m_clearValue;

    renderPassInfo.renderArea.extent = swapChainExtent;

    vkCmdBeginRenderPass(m_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void Frame::end()
{
    vkCmdEndRenderPass(m_commandBuffer);

    if (vkEndCommandBuffer(m_commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer!");
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffer;

    VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(Context::instance()->getDevice()->getGraphicsQueue(), 1, &submitInfo, m_inFlightFence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    VkSwapchainKHR swapChains[] = {Context::instance()->getSwapChain()->getVkSwapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &m_imageIndex;
    presentInfo.pResults = nullptr; // Optional

    vkQueuePresentKHR(Context::instance()->getDevice()->getPresentQueue(), &presentInfo);
}

void Frame::setClearValue(VkClearValue clearValue)
{
    m_clearValue = clearValue;
}

VkClearValue Frame::getClearVaule() const
{
    return m_clearValue;
}
