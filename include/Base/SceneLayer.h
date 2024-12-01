#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "Base/Layer.h"

class Node;
class SceneLayer : public Layer
{
public:
    SceneLayer(Window *window);
    ~SceneLayer() override;

    void recordCommandBuffer(Frame *frame) override;

    void updateUniformBuffer(Frame *frame);

    void createUniformBuffers();

    void setModelFile(const std::string_view &);

private:
    void updateDescriptorSets();

private:
    Node *m_rootNode;

    std::vector<VkDescriptorSet> m_descriptorSets;

    std::vector<VkBuffer> m_uniformBuffers;
    std::vector<VkDeviceMemory> m_uniformBuffersMemory;
    std::vector<void *> m_uniformBuffersMapped;
};