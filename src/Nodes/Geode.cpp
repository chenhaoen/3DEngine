#include <memory>
#include <algorithm>

#include "Nodes/Geode.h"
#include "Nodes/Geometry.h"

const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4};

Geode::Geode()
    : Node()
{
    Geometry *geometry = new Geometry();
    geometry->setVertices(vertices);
    geometry->setIndices(indices);
    m_geometices.push_back(geometry);
}

Geode::~Geode()
{
    std::for_each(m_geometices.begin(), m_geometices.end(), std::default_delete<Geometry>());
}

void Geode::addGeometry(Geometry *geometry)
{
    m_geometices.emplace_back(geometry);
}

void Geode::compile()
{
    for (auto geometry : m_geometices)
    {
        geometry->compile();
    }
}

void Geode::record(VkCommandBuffer commandBuffer)
{
    for (auto geometry : m_geometices)
    {
        geometry->record(commandBuffer);
    }
}