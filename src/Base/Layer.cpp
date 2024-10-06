#include "Base/Layer.h"

#include "Vk/DescriptorSetLayout.h"
#include "Vk/Pipeline.h"

Layer::Layer(Window *window)
    : m_window(window), m_pipeline(nullptr), m_descriptorSetLayout(nullptr)
{
}

Layer::~Layer()
{
    delete m_descriptorSetLayout;
    delete m_pipeline;
}
