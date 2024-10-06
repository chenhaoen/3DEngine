#include "Base/SceneLayer.h"

#include "Nodes/Node.h"
#include "Nodes/Group.h"
#include "Nodes/Geode.h"
#include "Nodes/Geometry.h"

#include "Vk/Pipeline.h"
#include "Vk/DescriptorSetLayout.h"

const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}};

const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0};

SceneLayer::SceneLayer(Window *window)
:Layer(window)
,m_rootNode(nullptr)
{
    m_descriptorSetLayout = new DescriptorSetLayout();
    m_pipeline = new Pipeline(m_descriptorSetLayout);

    Group* group = new Group();

    Geode* geode = new Geode();
    group->addChildren(geode);

    Geometry* geometry = new Geometry();
    geode->addGeometry(geometry);

    m_rootNode = group;

}

SceneLayer::~SceneLayer()
{
    delete m_rootNode;
}

void SceneLayer::recordCommandBuffer(Frame *frame) 
{
    if(!m_rootNode)
    {
        return;
    }
}
