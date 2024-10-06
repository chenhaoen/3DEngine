#pragma once

#include <vector>
#include <string>

#include <vulkan/vulkan.h>

class DescriptorSetLayout;
class Pipeline
{
public:
    Pipeline(DescriptorSetLayout *descriptorSetLayout);
    ~Pipeline();

    static std::vector<char> readFile(const std::string &filename);

    VkShaderModule createShaderModule(const std::vector<char> &code);

    VkPipeline getVkPipeline() const;
    VkPipelineLayout getVkPipelineLayout() const;

private:
    DescriptorSetLayout *m_descriptorSetLayout;

    VkPipelineLayout m_layout;
    VkPipeline m_graphicsPipeline;
};
