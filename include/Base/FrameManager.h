#pragma once

#include <cstdint>
#include <vector>
#include <list>

class Frame;
class Layer;
class LogicalDevice;
class CommandPool;
class SwapChain;
class RenderPass;
class FrameManager
{
public:
    FrameManager(LogicalDevice* device, SwapChain* swapChain, RenderPass* renderPass);
    ~FrameManager();

    void frame(const std::list<Layer*>& layers);
private:
    LogicalDevice* m_device;
    CommandPool* m_commandPool;
    SwapChain* m_swapChain;
    RenderPass* m_renderPass;

    std::vector<Frame *> m_frames;
    uint32_t m_currentFrame;
};