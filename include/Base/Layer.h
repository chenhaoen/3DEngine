#pragma once

class Frame;
class Instance;
class LogicalDevice;
class Window;
class SwapChain;
class Pipeline;
class DescriptorSetLayout;
class RenderPass;
class Layer
{
public:
    Layer(Instance *instance, LogicalDevice *device, Window *window, SwapChain *SwapChain, RenderPass *renderPass);
    virtual ~Layer() = 0;

    virtual void recordCommandBuffer(Frame *frame) = 0;

protected:
    Instance *m_instance;
    LogicalDevice *m_device;
    Window *m_window;
    SwapChain *m_swapChain;
    RenderPass *m_renderPass;

	Pipeline* m_pipeline;
	DescriptorSetLayout* m_descriptorSetLayout;
};