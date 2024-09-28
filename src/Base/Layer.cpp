#include "Base/Layer.h"

Layer::Layer(Window *window)
    : m_window(window), m_pipeline(nullptr), m_descriptorSetLayout(nullptr)
{
}

Layer::~Layer()
{
}
