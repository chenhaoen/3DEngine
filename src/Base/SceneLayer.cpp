#include "Base/SceneLayer.h"
#include "Base/Frame.h"
#include "Base/Context.h"

#include "Nodes/Node.h"
#include "Nodes/Group.h"
#include "Nodes/Geode.h"
#include "Nodes/Geometry.h"

#include "Vk/Pipeline.h"
#include "Vk/DescriptorSetLayout.h"
#include "Vk/SwapChain.h"

const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0};

SceneLayer::SceneLayer(Window *window)
    : Layer(window), m_rootNode(nullptr)
{
    m_descriptorSetLayout = new DescriptorSetLayout();
    m_pipeline = new Pipeline(m_descriptorSetLayout);

    m_geometry = new Geometry();
    m_geometry->setVertices(vertices);
    m_geometry->setIndices(indices);
}

SceneLayer::~SceneLayer()
{
    delete m_rootNode;
    delete m_geometry;
}

void SceneLayer::recordCommandBuffer(Frame *frame)
{
    vkCmdBindPipeline(frame->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->getVkPipeline());

    auto swapChainExtent = Context::instance()->getSwapChain()->getExtent();

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapChainExtent.width;
    viewport.height = (float)swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(frame->getCommandBuffer(), 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;
    vkCmdSetScissor(frame->getCommandBuffer(), 0, 1, &scissor);

    m_geometry->bind(frame->getCommandBuffer());

    vkCmdDrawIndexed(frame->getCommandBuffer(), static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}
