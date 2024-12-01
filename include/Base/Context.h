#pragma once

#include <vector>

class Instance;
class DebugUtilsMessenger;
class PhysicalDevice;
class LogicalDevice;
class Surface;
class SwapChain;
class RenderPass;
class Window;
class CommandPool;
class DescriptorPool;
class Descriptor;
class Context
{
public:
    Context();
    ~Context();

    static Context *instance();

    Instance* getInsance();
    Surface* getSurface();
    SwapChain* getSwapChain();
    LogicalDevice* getDevice();
    RenderPass* getRenderPass();
    CommandPool* getCommandPool();
    DescriptorPool* getDescriptorPool();

    void init();

    void setWindow(Window* window);

    const std::vector<const char *> &getInstanceExtensions() const;
	const std::vector<const char *> &getDeviceExtensions() const;
	const std::vector<const char *> &getInstanceLayers() const;

    std::vector<std::vector<Descriptor*>> m_descriptors;
private:
    static Context* g_context;

    Window* m_window;
    Instance *m_instance;
    Surface *m_surface;
    SwapChain *m_swapChain;
    DebugUtilsMessenger *m_debugUtilsMessenger;
    PhysicalDevice *m_physicalDevice;
    LogicalDevice *m_logicalDevice;
    RenderPass *m_renderPass;
    CommandPool* m_commandPool;
    DescriptorPool* m_descriptorPool;

    std::vector<const char *> m_instanceExtensions;
    std::vector<const char *> m_instanceLayers;
    std::vector<const char *> m_deviceExtensions;
};