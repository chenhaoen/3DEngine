#pragma once

#include "Base/Layer.h"

class Node;
class Geometry;
class SceneLayer : public Layer
{
public:
    SceneLayer(Window *window);
    ~SceneLayer() override;

    void recordCommandBuffer(Frame *frame) override;

private:
    Node *m_rootNode;

    Geometry* m_geometry;
};