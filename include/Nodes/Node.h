#pragma once

#include <vulkan/vulkan.h>

class Node
{
public:
	Node();
	virtual ~Node();

	virtual void compile();

	virtual void record(VkCommandBuffer);
};
