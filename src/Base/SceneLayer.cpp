#include <stdexcept>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Base/SceneLayer.h"
#include "Base/Frame.h"
#include "Base/Context.h"
#include "Base/Application.h"

#include "Nodes/Node.h"
#include "Nodes/Group.h"
#include "Nodes/Geode.h"
#include "Nodes/Geometry.h"

#include "Vk/Pipeline.h"
#include "Vk/DescriptorSetLayout.h"
#include "Vk/SwapChain.h"
#include "Vk/DescriptorPool.h"
#include "Vk/LogicalDevice.h"

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

    createUniformBuffers();

    std::vector<VkDescriptorSetLayout> layouts(Application::maxFrameCount(), m_descriptorSetLayout->getVkDescriptorSetLayout());
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = Context::instance()->getDescriptorPool()->getVkDescriptorPool();
    allocInfo.descriptorSetCount = static_cast<uint32_t>(Application::maxFrameCount());
    allocInfo.pSetLayouts = layouts.data();

    m_descriptorSets.resize(Application::maxFrameCount());
    if (vkAllocateDescriptorSets(Context::instance()->getDevice()->getVkDevice(), &allocInfo, m_descriptorSets.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < Application::maxFrameCount(); i++)
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = m_uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = m_descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;

        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;

        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.pImageInfo = nullptr;       // Optional
        descriptorWrite.pTexelBufferView = nullptr; // Optional

        vkUpdateDescriptorSets(Context::instance()->getDevice()->getVkDevice(), 1, &descriptorWrite, 0, nullptr);
    }

    m_geometry = new Geometry();
    m_geometry->setVertices(vertices);
    m_geometry->setIndices(indices);
}

SceneLayer::~SceneLayer()
{
    delete m_rootNode;
    delete m_geometry;

    for (uint32_t i = 0; i < Application::maxFrameCount(); ++i)
    {
        vkDestroyBuffer(Context::instance()->getDevice()->getVkDevice(), m_uniformBuffers[i], nullptr);
        vkFreeMemory(Context::instance()->getDevice()->getVkDevice(), m_uniformBuffersMemory[i], nullptr);
    }
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

    updateUniformBuffer(frame);

    vkCmdBindDescriptorSets(frame->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->getVkPipelineLayout(),
                            0, 1, &m_descriptorSets[frame->getIndex()], 0, nullptr);

    vkCmdDrawIndexed(frame->getCommandBuffer(), static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}

void SceneLayer::updateUniformBuffer(Frame *frame)
{
    auto swapChainExtent = Context::instance()->getSwapChain()->getExtent();

    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);

    ubo.proj[1][1] *= -1;

    memcpy(m_uniformBuffersMapped[frame->getIndex()], &ubo, sizeof(ubo));
}

void SceneLayer::createUniformBuffers()
{
    m_uniformBuffers.resize(Application::maxFrameCount());
    m_uniformBuffersMemory.resize(Application::maxFrameCount());
    m_uniformBuffersMapped.resize(Application::maxFrameCount());
    for (uint32_t i = 0; i < Application::maxFrameCount(); ++i)
    {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);
        Geometry::createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniformBuffers[i], m_uniformBuffersMemory[i]);

        vkMapMemory(Context::instance()->getDevice()->getVkDevice(), m_uniformBuffersMemory[i], 0, bufferSize, 0, &m_uniformBuffersMapped[i]);
    }
}
