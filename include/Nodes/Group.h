#pragma once

#include <vector>

#include "Nodes/Node.h"
#include "Base/Exports.h"

class BASE_API Group : public Node
{
public:
    Group();
    ~Group();

    void addChildren(Node* children);

    void compile() override;

    void record(VkCommandBuffer) override;
    
private:
    std::vector<Node *> m_childrens;
};