#include <algorithm>
#include <memory>

#include "Base/LayerManager.h"
#include "Base/OverlayLayer.h"

LayerManager::LayerManager(Window *window)
    : m_overlayLayer(nullptr), m_sceneLayer(nullptr)
{
    m_overlayLayer = new OverlayLayer(window);

    m_layers.push_back(m_overlayLayer);
}

LayerManager::~LayerManager()
{
    std::for_each(m_layers.begin(), m_layers.end(), std::default_delete<Layer>());
}

 void LayerManager::recordCommandBuffer(Frame *frame)
 {
    for(Layer* layer : m_layers)
    {
        layer->recordCommandBuffer(frame);
    }
 }
