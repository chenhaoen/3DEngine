#include <stdexcept>

#include "Base/Context.h"
#include "Base/Window.h"
#include "Base/Application.h"

#include "Vk/Instance.h"
#include "Vk/DebugUtilsMessenger.h"
#include "Vk/LogicalDevice.h"
#include "Vk/PhysicalDevice.h"
#include "Vk/Surface.h"
#include "Vk/SwapChain.h"
#include "Vk/RenderPass.h"
#include "Vk/CommandPool.h"
#include "Vk/DescriptorPool.h"

Context *Context::g_context = nullptr;

Context::Context()
    : m_window(nullptr)
{
    g_context = this;
}

Context::~Context()
{
    delete m_commandPool;
    delete m_descriptorPool;
    delete m_renderPass;
    delete m_swapChain;
    delete m_logicalDevice;
    delete m_physicalDevice;
    delete m_surface;
    delete m_debugUtilsMessenger;
    delete m_instance;
}

Instance *Context::getInsance()
{
    return m_instance;
}

Surface *Context::getSurface()
{
    return m_surface;
}

SwapChain *Context::getSwapChain()
{
    return m_swapChain;
}

LogicalDevice *Context::getDevice()
{
    return m_logicalDevice;
}

RenderPass *Context::getRenderPass()
{
    return m_renderPass;
}

CommandPool *Context::getCommandPool()
{
    return m_commandPool;
}

DescriptorPool *Context::getDescriptorPool()
{
    return m_descriptorPool;
}

void Context::init()
{
    if (!m_window)
    {
        return;
    }

    const std::vector<const char *> &windowInstanceExtensions = Window::GetRequiredInstanceExtensions();
    m_instanceExtensions.insert(m_instanceExtensions.end(), windowInstanceExtensions.begin(), windowInstanceExtensions.end());
    m_deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    if (Application::instance()->debug())
    {
        m_instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        m_instanceLayers.push_back("VK_LAYER_KHRONOS_validation");
    }

    m_instance = new Instance();

    m_surface = new Surface(m_instance, m_window);

    if (Application::instance()->debug())
    {
        m_debugUtilsMessenger = new DebugUtilsMessenger();
    }

    m_physicalDevice = m_instance->getBestPhysicalDevice(m_surface);
    if (m_physicalDevice == nullptr)
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    m_logicalDevice = new LogicalDevice(m_physicalDevice);

    m_swapChain = new SwapChain(m_logicalDevice, m_physicalDevice, m_surface, m_window);

    m_renderPass = new RenderPass(m_logicalDevice, m_swapChain);

    m_swapChain->setRenderPass(m_renderPass);
    m_swapChain->createFrameBuffers();

    m_commandPool = new CommandPool();

    m_descriptorPool = new DescriptorPool();
}

const std::vector<const char *> &Context::getInstanceExtensions() const
{
    return m_instanceExtensions;
}

const std::vector<const char *> &Context::getDeviceExtensions() const
{
    return m_deviceExtensions;
}

const std::vector<const char *> &Context::getInstanceLayers() const
{
    return m_instanceLayers;
}

Context *Context::instance()
{
    return g_context;
}

void Context::setWindow(Window *window)
{
    m_window = window;
}