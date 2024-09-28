#pragma once

#include "Base/Layer.h"

class Node;
class SceneLayer : public Layer
{
    SceneLayer();

    private:
    Node* m_rootNode;
};