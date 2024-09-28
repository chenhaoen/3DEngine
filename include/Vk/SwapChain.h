#pragma once

#include <vector>

#include <vulkan/vulkan.h>

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class PhysicalDevice;
class Surface;
class Window;
class LogicalDevice;
class RenderPass;
class SwapChain
{
public:
   SwapChain(
        LogicalDevice* device,
        PhysicalDevice* physicalDevice,
        Surface* surface,
        Window* window);

   ~SwapChain();

   void create();
   void createImageViews();
   void createFrameBuffers();

   void cleanup();

   void setRenderPass(RenderPass* renderPass);

   VkFormat getImageFormat() const;
   VkExtent2D getExtent() const;
   VkSwapchainKHR getVkSwapChain() const;

   VkFramebuffer getFrameBuffer(const int index);

   uint32_t getNextImageIndex(VkSemaphore semaphore);
private:

   SwapChainSupportDetails querySwapChainSupport();

   VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
   VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
   VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

private:
    Window* m_window;
    PhysicalDevice* m_physicalDevice;
    Surface* m_surface;
    LogicalDevice* m_device;
    RenderPass* m_renderPass;

    VkSwapchainKHR m_vkSwapChain;

    std::vector<VkImage> m_images;
    std::vector<VkImageView> m_imageViews;
    VkFormat m_imageFormat;
    VkExtent2D m_extent;

    std::vector<VkFramebuffer> m_frameBuffers;
};

