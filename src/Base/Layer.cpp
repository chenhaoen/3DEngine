#include "Base/Layer.h"

Layer::Layer(Instance* instance, LogicalDevice *device, Window *window, SwapChain* swapChain, RenderPass *renderPass)
:m_instance(instance)
,m_device(device)
,m_window(window)
,m_swapChain(swapChain)
,m_pipeline(nullptr)
,m_descriptorSetLayout(nullptr)
,m_renderPass(renderPass)
{

}

Layer::~Layer()
{

}
