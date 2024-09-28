#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

class CommandPool;
class Frame
{
public:
    Frame(CommandPool *commandPool);
    ~Frame();

    VkCommandBuffer getCommandBuffer() const;

    void begin();
    void end();

    void setClearValue(VkClearValue clearValue);
    VkClearValue getClearVaule() const;
private:
    CommandPool *m_commandPool;

    VkCommandBuffer m_commandBuffer;

    VkSemaphore m_imageAvailableSemaphore;
    VkSemaphore m_renderFinishedSemaphore;
    VkFence m_inFlightFence;

    uint32_t m_imageIndex;

    VkClearValue m_clearValue;
};