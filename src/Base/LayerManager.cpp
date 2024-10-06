#include <algorithm>
#include <memory>

#include "Base/LayerManager.h"
#include "Base/OverlayLayer.h"
#include "Base/SceneLayer.h"

LayerManager::LayerManager(Window *window)
    : m_overlayLayer(nullptr), m_sceneLayer(nullptr)
{
    m_overlayLayer = new OverlayLayer(window);

    m_sceneLayer = new SceneLayer(window);

    m_layers.push_back(m_overlayLayer);
    m_layers.push_back(m_sceneLayer);
}

LayerManager::~LayerManager()
{
    std::for_each(m_layers.begin(), m_layers.end(), std::default_delete<Layer>());
}

void LayerManager::recordCommandBuffer(Frame *frame)
{
    for (Layer *layer : m_layers)
    {
        layer->recordCommandBuffer(frame);
    }
}
