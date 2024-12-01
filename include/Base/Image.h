#pragma once

#include <vulkan/vulkan.h>

VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

void createImage(uint32_t width,
                 uint32_t height,
                 VkFormat format,
                 VkImageTiling tiling,
                 VkImageUsageFlags usage,
                 VkMemoryPropertyFlags properties,
                 VkImage &image,
                 VkDeviceMemory &imageMemory);

VkFormat findDepthFormat();

VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
