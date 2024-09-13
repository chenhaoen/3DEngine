#pragma once

#include <array>

#include <vulkan/vulkan.h>

#include <glm/glm.hpp>

#include "Base/Application.h"

class Pipeline;
class LogicalDevice;
class SwapChain;
class RenderPass;

struct Vertex
{
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription getBindingDescription();
	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
};

const std::vector<Vertex> vertices = {
	{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
	{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

class Node
{
public:
	Node(LogicalDevice* device, SwapChain* swapChain);
	~Node();

	void drawFrame(RenderPass* renderPass, Pipeline* pipeline);

	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, RenderPass* renderPass, Pipeline* pipeline);

	void createVertexBuffer();
private:

	LogicalDevice* m_device;
	SwapChain* m_swapChain;

	VkCommandPool m_commandPool;
	std::vector<VkCommandBuffer> m_commandBuffers;

	std::vector<VkSemaphore> m_imageAvailableSemaphores;
	std::vector<VkSemaphore> m_renderFinishedSemaphores;
	std::vector<VkFence> m_inFlightFences;

	uint32_t m_currentFrame;

	VkBuffer m_vertexBuffer;
	VkDeviceMemory m_vertexBufferMemory;
};

