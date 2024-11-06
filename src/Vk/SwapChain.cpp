#include <algorithm>
#include <stdexcept>
#include <limits>
#include <array>

#include "Vk/SwapChain.h"
#include "Vk/PhysicalDevice.h"
#include "Vk/Surface.h"
#include "Vk/LogicalDevice.h"
#include "Vk/RenderPass.h"

#include "Base/Window.h"
#include "Base/Context.h"
#include "Base/Image.h"

bool hasStencilComponent(VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

SwapChain::SwapChain(
    LogicalDevice *device,
    PhysicalDevice *physicalDevice,
    Surface *surface,
    Window *window)
    : m_window(window), m_physicalDevice(physicalDevice), m_surface(surface), m_device(device), m_renderPass(nullptr)
{
    create();
    createImageViews();
    createDepthResources();
}

SwapChain::~SwapChain()
{
    cleanup();
}

void SwapChain::createFrameBuffers()
{
    m_frameBuffers.resize(m_imageViews.size());
    for (size_t i = 0; i < m_frameBuffers.size(); i++)
    {
        std::array<VkImageView, 2> attachments = {
            m_imageViews[i], m_depthImageView};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderPass->getVkRenderPass();
        framebufferInfo.attachmentCount = attachments.size();
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = m_extent.width;
        framebufferInfo.height = m_extent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(m_device->getVkDevice(), &framebufferInfo, nullptr, &m_frameBuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void SwapChain::createDepthResources()
{
    VkFormat depthFormat = findDepthFormat();

    createImage(m_extent.width, m_extent.height, depthFormat,
                VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_depthImage, m_depthImageMemory);
    m_depthImageView = createImageView(m_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void SwapChain::create()
{
    SwapChainSupportDetails details = querySwapChainSupport();

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(details.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(details.presentModes);
    VkExtent2D extent = chooseSwapExtent(details.capabilities);

    uint32_t imageCount = details.capabilities.minImageCount + 1;

    if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount)
    {
        imageCount = details.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_surface->getVkSurface();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    const uint32_t graphicsFamilyIndex = m_physicalDevice->getGraphicsFamilyIndex();
    const uint32_t presentFamilyIndex = m_physicalDevice->getPresentFamilyIndex();
    uint32_t queueFamilyIndices[] = {graphicsFamilyIndex, presentFamilyIndex};

    if (graphicsFamilyIndex != presentFamilyIndex)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;     // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = details.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_device->getVkDevice(), &createInfo, nullptr, &m_vkSwapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create swap chain!");
    }

    m_imageFormat = surfaceFormat.format;
    m_extent = extent;
    vkGetSwapchainImagesKHR(m_device->getVkDevice(), m_vkSwapChain, &imageCount, nullptr);
    m_images.resize(imageCount);
    vkGetSwapchainImagesKHR(m_device->getVkDevice(), m_vkSwapChain, &imageCount, m_images.data());
}

void SwapChain::createImageViews()
{
    m_imageViews.resize(m_images.size());
    for (size_t i = 0; i < m_imageViews.size(); i++)
    {
        m_imageViews[i] = createImageView(m_images[i], m_imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

void SwapChain::cleanup()
{
    for (auto imageView : m_imageViews)
    {
        vkDestroyImageView(m_device->getVkDevice(), imageView, nullptr);
    }

    for (auto framebuffer : m_frameBuffers)
    {
        vkDestroyFramebuffer(m_device->getVkDevice(), framebuffer, nullptr);
    }

    vkDestroySwapchainKHR(m_device->getVkDevice(), m_vkSwapChain, nullptr);

    vkDestroyImage(Context::instance()->getDevice()->getVkDevice(), m_depthImage, nullptr);
    vkFreeMemory(Context::instance()->getDevice()->getVkDevice(), m_depthImageMemory, nullptr);
    vkDestroyImageView(Context::instance()->getDevice()->getVkDevice(), m_depthImageView, nullptr);
}

void SwapChain::setRenderPass(RenderPass *renderPass)
{
    m_renderPass = renderPass;
}

VkFormat SwapChain::getImageFormat() const
{
    return m_imageFormat;
}

VkExtent2D SwapChain::getExtent() const
{
    return m_extent;
}

VkSwapchainKHR SwapChain::getVkSwapChain() const
{
    return m_vkSwapChain;
}

VkFramebuffer SwapChain::getFrameBuffer(const int index)
{
    return m_frameBuffers[index];
}

uint32_t SwapChain::getNextImageIndex(VkSemaphore semaphore)
{
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(m_device->getVkDevice(),
                                            m_vkSwapChain, UINT64_MAX, semaphore, VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        return imageIndex;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    return imageIndex;
}

SwapChainSupportDetails SwapChain::querySwapChainSupport()
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice->getVkPhysicalDevice(), m_surface->getVkSurface(), &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice->getVkPhysicalDevice(), m_surface->getVkSurface(), &formatCount, nullptr);

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice->getVkPhysicalDevice(), m_surface->getVkSurface(), &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice->getVkPhysicalDevice(), m_surface->getVkSurface(), &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice->getVkPhysicalDevice(), m_surface->getVkSurface(),
                                                  &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    for (const auto &availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
    for (const auto &availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        m_window->getFrameBufferSize(width, height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)};

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}
