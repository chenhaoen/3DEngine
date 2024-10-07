#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

class CommandPool;
class Frame
{
public:
    Frame(uint32_t index);
    ~Frame();

    VkCommandBuffer getCommandBuffer() const;

    uint32_t getIndex() const;

    void begin();
    void end();

    void setClearValue(VkClearValue clearValue);
    VkClearValue getClearVaule() const;

private:
    VkCommandBuffer m_commandBuffer;

    VkSemaphore m_imageAvailableSemaphore;
    VkSemaphore m_renderFinishedSemaphore;
    VkFence m_inFlightFence;

    uint32_t m_imageIndex;

    VkClearValue m_clearValue;

    const uint32_t m_index;
};