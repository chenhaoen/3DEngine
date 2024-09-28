#pragma once

#include <vulkan/vulkan.h>

class SwapChain;
class LogicalDevice;
class RenderPass
{
public:
	RenderPass(LogicalDevice* device, SwapChain* swapChain);
	~RenderPass();

	VkRenderPass getVkRenderPass() const;

private:
	VkRenderPass m_renderPass;
	LogicalDevice* m_device;
};

