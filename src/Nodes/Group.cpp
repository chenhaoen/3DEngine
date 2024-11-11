#include <algorithm>
#include <memory>

#include "Nodes/Group.h"

Group::Group()
    : Node()
{
}

Group::~Group()
{
    std::for_each(m_childrens.begin(), m_childrens.end(), std::default_delete<Node>());
}

void Group::addChildren(Node *children)
{
    m_childrens.push_back(children);
}

void Group::compile()
{
    for (auto children : m_childrens)
    {
        children->compile();
    }
}

void Group::record(VkCommandBuffer commandBuffer)
{
    for (auto children : m_childrens)
    {
        children->record(commandBuffer);
    }
}