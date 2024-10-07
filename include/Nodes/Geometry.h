#pragma once

#include <vector>
#include <array>

#include <glm/glm.hpp>

#include <vulkan/vulkan.h>

struct Vertex
{
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription getBindingDescription();
	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
};

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

class Geometry
{
public:
	Geometry();
	~Geometry();

	void setVertices(const std::vector<Vertex> &vertices);

	void setIndices(const std::vector<uint16_t> indices);

	void bind(VkCommandBuffer commandBuffer);

	void createVertexBuffer();

	void createIndexBuffer();

	void createUniformBuffers();

	void updateUniformBuffer(uint32_t currentFrame);

	static void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);

	static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

private:
	std::vector<Vertex> m_vertices;

	std::vector<uint16_t> m_indices;

	VkBuffer m_vertexBuffer;
	VkDeviceMemory m_vertexBufferMemory;

	VkBuffer m_indexBuffer;
	VkDeviceMemory m_indexBufferMemory;

	VkCommandPool m_commandPool;
};