#pragma once

#include <vector>
#include <array>

#include <glm/glm.hpp>

#include <vulkan/vulkan.h>

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	static VkVertexInputBindingDescription getBindingDescription();
	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
};

struct UniformBufferObject
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

class Geometry
{
public:
	Geometry();
	~Geometry();

	void setVertices(const std::vector<Vertex> &vertices);

	void setIndices(const std::vector<uint16_t> indices);
	
	void setTextureFile(const std::string_view& file);

	void compile();

	void record(VkCommandBuffer commandBuffer);

	void createTextureImage();

	void createTextureImageView();

	void createVertexBuffer();

	void createIndexBuffer();

	void createUniformBuffers();

	void updateUniformBuffer(uint32_t currentFrame);

	static void createBuffer(VkDeviceSize size,
							 VkBufferUsageFlags usage,
							 VkMemoryPropertyFlags properties,
							 VkBuffer &buffer,
							 VkDeviceMemory &bufferMemory);

	static void copyBuffer(VkBuffer srcBuffer,
						   VkBuffer dstBuffer,
						   VkDeviceSize size);

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void createTextureSampler();

	VkImage m_textureImage;
	VkDeviceMemory m_textureBufferMemory;
	VkImageView m_textureImageView;
	VkSampler m_textureSampler;

private:
	std::vector<Vertex> m_vertices;

	std::vector<uint16_t> m_indices;

	VkBuffer m_vertexBuffer;
	VkDeviceMemory m_vertexBufferMemory;

	VkBuffer m_indexBuffer;
	VkDeviceMemory m_indexBufferMemory;

	VkCommandPool m_commandPool;

	std::string_view m_textureFile = "models/viking_room.png";
};