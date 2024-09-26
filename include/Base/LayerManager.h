#pragma once

#include <list>

class Layer;
class OverlayLayer;
class SceneLayer;
class Instance;
class LogicalDevice;
class Window;
class RenderPass;
class SwapChain;
class Frame;
class LayerManager
{
public:
    LayerManager(Instance *instance, LogicalDevice *device, Window *window, SwapChain *swapChain, RenderPass *renderPass);
    ~LayerManager();

    void recordCommandBuffer(Frame *frame);

private:
    std::list<Layer *> m_layers;

    OverlayLayer *m_overlayLayer;
    SceneLayer *m_sceneLayer;
};