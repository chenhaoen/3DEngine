#pragma once

#include <vector>
#include <string>

#include <vulkan/vulkan.h>

class LogicalDevice;
class SwapChain;
class RenderPass;
class Pipeline
{
public:
    Pipeline(LogicalDevice* device, SwapChain* swapChain, RenderPass* renderPass);
    ~Pipeline();

    static std::vector<char> readFile(const std::string& filename);

    VkShaderModule createShaderModule(const std::vector<char>& code);

    VkPipeline getVkPipeline() const;
private:
    LogicalDevice* m_device;

    VkPipelineLayout m_layout;
    VkPipeline m_graphicsPipeline;
};

