#pragma once

#include <cstdint>
#include <vector>

class Frame;
class LayerManager;
class DescriptorPool;
class FrameManager
{
public:
    FrameManager();
    ~FrameManager();

    void frame(LayerManager* layerManager);
private:
    DescriptorPool* m_descriptorPool;

    std::vector<Frame *> m_frames;
    uint32_t m_currentFrame;
};