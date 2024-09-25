#include <stdexcept>

#include "Base/Frame.h"
#include "Base/LogicalDevice.h"
#include "Base/CommandPool.h"
#include "Base/SwapChain.h"
#include "Base/RenderPass.h"

Frame::Frame(LogicalDevice *device, CommandPool *commandPool, SwapChain *swapChain, RenderPass *renderPass)
    : m_device(device), m_commandPool(commandPool), m_swapChain(swapChain), m_imageIndex(0), m_renderPass(renderPass)
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

    if (vkAllocateCommandBuffers(device->getVkDevice(), &allocInfo, &m_commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    if (vkCreateSemaphore(device->getVkDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(device->getVkDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS ||
        vkCreateFence(device->getVkDevice(), &fenceInfo, nullptr, &m_inFlightFence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create semaphores!");
    }

    m_clearValue.color = {0.0f, 0.0f, 0.0f, 1.0f};
}

Frame::~Frame()
{
    vkDestroySemaphore(m_device->getVkDevice(), m_imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(m_device->getVkDevice(), m_renderFinishedSemaphore, nullptr);
    vkDestroyFence(m_device->getVkDevice(), m_inFlightFence, nullptr);
}

VkCommandBuffer Frame::getCommandBuffer() const
{
    return m_commandBuffer;
}

void Frame::begin()
{
    vkWaitForFences(m_device->getVkDevice(), 1, &m_inFlightFence, VK_TRUE, UINT64_MAX);

    vkResetFences(m_device->getVkDevice(), 1, &m_inFlightFence);

    m_imageIndex = m_swapChain->getNextImageIndex(m_imageAvailableSemaphore);

    vkResetCommandBuffer(m_commandBuffer, 0);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;                  // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(m_commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkExtent2D swapChainExtent = m_swapChain->getExtent();

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_renderPass->getVkRenderPass();
    renderPassInfo.framebuffer = m_swapChain->getFrameBuffer(m_imageIndex);

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

    if (vkQueueSubmit(m_device->getGraphicsQueue(), 1, &submitInfo, m_inFlightFence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    VkSwapchainKHR swapChains[] = {m_swapChain->getVkSwapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &m_imageIndex;
    presentInfo.pResults = nullptr; // Optional

    vkQueuePresentKHR(m_device->getPresentQueue(), &presentInfo);
}

void Frame::setClearValue(VkClearValue clearValue)
{
    m_clearValue = clearValue;
}

VkClearValue Frame::getClearVaule() const
{
    return m_clearValue;
}
