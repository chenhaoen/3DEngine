#pragma once

#include <vulkan/vulkan.h>

#include "Base/Layer.h"

class OverlayLayer : public Layer
{
public:
    OverlayLayer(Window *window);
    ~OverlayLayer() override;

    void recordCommandBuffer(Frame *frame) override;

private:
    VkDescriptorPool m_descriptorPool;
};