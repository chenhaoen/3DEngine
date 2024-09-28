#pragma once

#include <cstdint>
#include <vector>

class Frame;
class CommandPool;
class LayerManager;
class FrameManager
{
public:
    FrameManager();
    ~FrameManager();

    void frame(LayerManager* layerManager);
private:
    CommandPool* m_commandPool;

    std::vector<Frame *> m_frames;
    uint32_t m_currentFrame;
};