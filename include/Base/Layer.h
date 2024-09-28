#pragma once

class Frame;
class Window;
class Pipeline;
class DescriptorSetLayout;
class Layer
{
public:
    Layer(Window *window);
    virtual ~Layer() = 0;

    virtual void recordCommandBuffer(Frame *frame) = 0;

protected:
    Window *m_window;

	Pipeline* m_pipeline;
	DescriptorSetLayout* m_descriptorSetLayout;
};