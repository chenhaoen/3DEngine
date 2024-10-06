#pragma once

#include <vector>

#include "Nodes/Node.h"

class Geometry;
class Geode : public Node
{
public:
    Geode();
    ~Geode();

    void addGeometry(Geometry* geometry);

private:
    std::vector<Geometry *> m_geometices;
};