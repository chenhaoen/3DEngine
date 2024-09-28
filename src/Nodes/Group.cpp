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