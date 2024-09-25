#pragma once

#include "Base/Layer.h"

class OverlayLayer : public Layer
{
public:
    OverlayLayer(Instance *instance, LogicalDevice *device, Window *window, SwapChain *swapChain, RenderPass* renderPass);
    ~OverlayLayer();

    void recordCommandBuffer(Frame *frame) override;

private:
    VkDescriptorPool m_descriptorPool;
};