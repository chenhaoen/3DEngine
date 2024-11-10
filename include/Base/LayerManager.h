#pragma once

#include <list>

class Layer;
class OverlayLayer;
class SceneLayer;
class Window;
class Frame;
class LayerManager
{
public:
    LayerManager(Window *window);
    ~LayerManager();

    void recordCommandBuffer(Frame *frame);

    void setModelFile(const std::string_view&);
private:
    std::list<Layer *> m_layers;

    OverlayLayer *m_overlayLayer;
    SceneLayer *m_sceneLayer;
};