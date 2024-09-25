#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

class LogicalDevice;
class CommandPool;
class SwapChain;
class RenderPass;
class Frame
{
public:
    Frame(
        LogicalDevice *device,
        CommandPool *commandPool,
        SwapChain *swapChain,
        RenderPass *renderPass);
    ~Frame();

    VkCommandBuffer getCommandBuffer() const;

    void begin();
    void end();

    void setClearValue(VkClearValue clearValue);
    VkClearValue getClearVaule() const;
private:
    LogicalDevice *m_device;
    CommandPool *m_commandPool;
    SwapChain *m_swapChain;
    RenderPass *m_renderPass;

    VkCommandBuffer m_commandBuffer;

    VkSemaphore m_imageAvailableSemaphore;
    VkSemaphore m_renderFinishedSemaphore;
    VkFence m_inFlightFence;

    uint32_t m_imageIndex;

    VkClearValue m_clearValue;
};