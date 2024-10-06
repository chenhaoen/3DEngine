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


class Geometry
{
public:
    Geometry() = default;
    ~Geometry() = default;

	void setVertices(const std::vector<Vertex>& vertices);

private:
    std::vector<Vertex> m_vertices;

	std::vector<uint16_t> m_indices;
};