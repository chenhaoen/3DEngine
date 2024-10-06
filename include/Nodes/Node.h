#pragma once

#include <array>

#include <vulkan/vulkan.h>

#include <glm/glm.hpp>

#include "Base/Application.h"

class Pipeline;
class LogicalDevice;
class SwapChain;
class RenderPass;
class DescriptorSetLayout;


struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};



class BASE_API Node
{
public:
	Node();
	virtual ~Node();

private:
	void createVertexBuffer();

	void createIndexBuffer();

	void createUniformBuffers();

	void updateUniformBuffer(uint32_t currentFrame);

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void createDescriptorPool();

	void createDescriptorSets();
private:
	LogicalDevice *m_device;
	SwapChain *m_swapChain;

	VkCommandPool m_commandPool;
	std::vector<VkCommandBuffer> m_commandBuffers;

	VkBuffer m_vertexBuffer;
	VkDeviceMemory m_vertexBufferMemory;

	VkBuffer m_indexBuffer;
	VkDeviceMemory m_indexBufferMemory;

	std::vector<VkBuffer> m_uniformBuffers;
	std::vector<VkDeviceMemory> m_uniformBuffersMemory;
	std::vector<void*> m_uniformBuffersMapped;

	VkDescriptorPool m_descriptorPool;
	std::vector<VkDescriptorSet> m_descriptorSets;
	DescriptorSetLayout* m_descriptorSetLayout;
};
