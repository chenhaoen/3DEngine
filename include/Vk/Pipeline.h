#pragma once

#include <vector>
#include <string>

#include <vulkan/vulkan.h>

class LogicalDevice;
class SwapChain;
class RenderPass;
class DescriptorSetLayout;
class Pipeline
{
public:
    Pipeline(
        LogicalDevice *device,
        SwapChain *swapChain,
        RenderPass *renderPass,
        DescriptorSetLayout *descriptorSetLayout);
    ~Pipeline();

    static std::vector<char> readFile(const std::string &filename);

    VkShaderModule createShaderModule(const std::vector<char> &code);

    VkPipeline getVkPipeline() const;
    VkPipelineLayout getVkPipelineLayout() const;

private:
    LogicalDevice *m_device;
    DescriptorSetLayout *m_descriptorSetLayout;

    VkPipelineLayout m_layout;
    VkPipeline m_graphicsPipeline;
};
