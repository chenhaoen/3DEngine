#pragma once

#include <vector>

#include "Nodes/Node.h"

class Geometry;
class Geode : public Node
{
public:
    Geode();
    ~Geode();

    void addGeometry(Geometry *geometry);

    void compile() override;

    void record(VkCommandBuffer) override;

private:
    std::vector<Geometry *> m_geometices;
};