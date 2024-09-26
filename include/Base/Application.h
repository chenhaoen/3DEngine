#pragma once

#include <string_view>
#include <vector>

#include "Base/Exports.h"

class Window;
class Instance;
class DebugUtilsMessenger;
class PhysicalDevice;
class LogicalDevice;
class Surface;
class SwapChain;
class Event;
class FrameManager;
class LayerManager;
class RenderPass;

class BASE_API Application
{
public:
	Application(const std::string_view &name, const bool debug);
	~Application();

	int exec();

	bool debug() const;

	const std::vector<const char *> &getInstanceExtensions() const;
	const std::vector<const char *> &getDeviceExtensions() const;
	const std::vector<const char *> &getInstanceLayers() const;
	const std::string_view &getName() const;

	void onEvent(Event &event);

	static constexpr int maxFrameCount() { return 2; };

	static Application *instance();

private:
	Window *m_mainWindow;
	Instance *m_instance;
	Surface *m_surface;
	SwapChain *m_swapChain;
	DebugUtilsMessenger *m_debugUtilsMessenger;
	PhysicalDevice *m_physicalDevice;
	LogicalDevice *m_logicalDevice;
	RenderPass *m_renderPass;
	FrameManager *m_frameManager;
	LayerManager *m_layerManager;

	std::vector<const char *> m_instanceExtensions;
	std::vector<const char *> m_instanceLayers;
	std::vector<const char *> m_deviceExtensions;

	bool m_debug;
	std::string_view m_name;

	static Application *g_instance;
};