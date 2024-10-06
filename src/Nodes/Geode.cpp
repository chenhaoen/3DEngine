#include <memory>
#include <algorithm>

#include "Nodes/Geode.h"
#include "Nodes/Geometry.h"

Geode::Geode()
    : Node()
{
}

Geode::~Geode()
{
    std::for_each(m_geometices.begin(), m_geometices.end(), std::default_delete<Geometry>());
}

void Geode::addGeometry(Geometry *geometry)
{
    m_geometices.emplace_back(geometry);
}