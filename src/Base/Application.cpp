#include <stdexcept>
#include <iostream>

#include "Base/Application.h"
#include "Base/Window.h"
#include "Base/Instance.h"
#include "Base/DebugUtilsMessenger.h"
#include "Base/LogicalDevice.h"
#include "Base/PhysicalDevice.h"
#include "Base/Surface.h"
#include "Base/SwapChain.h"
#include "Base/Node.h"
#include "Base/Event.h"
#include "Base/RenderPass.h"
#include "Base/Pipeline.h"
#include "Base/DescriptorSetLayout.h"

Application* Application::g_instance = nullptr;

Application::Application(
	const std::string_view& name,
	const bool debug)
	:m_name(name)
	, m_debug(debug)
	, m_debugUtilsMessenger(nullptr)
{
	if (g_instance != nullptr)
	{
		throw std::runtime_error("Application already exists!");
	}

	g_instance = this;

	m_mainWindow = new Window(1600, 1200, m_name);

	const std::vector<const char*>& windowInstanceExtensions = Window::GetRequiredInstanceExtensions();
	m_instanceExtensions.insert(m_instanceExtensions.end(), windowInstanceExtensions.begin(), windowInstanceExtensions.end());
	m_deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	if (m_debug)
	{
		m_instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		m_instanceLayers.push_back("VK_LAYER_KHRONOS_validation");
	}

	m_instance = new Instance();

	m_surface = new Surface(m_instance,m_mainWindow);

	if (m_debug)
	{
		m_debugUtilsMessenger = new DebugUtilsMessenger(m_instance);
	}

	m_physicalDevice = m_instance->getBestPhysicalDevice(m_surface);
	if (m_physicalDevice == nullptr)
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}

	m_logicalDevice = new LogicalDevice(m_physicalDevice);

	m_swapChain = new SwapChain(m_logicalDevice, m_physicalDevice, m_surface, m_mainWindow);

	m_renderPass = new RenderPass(m_logicalDevice, m_swapChain);
	m_descriptorSetLayout = new DescriptorSetLayout(m_logicalDevice);
	m_pipeline = new Pipeline(m_logicalDevice, m_swapChain, m_renderPass, m_descriptorSetLayout);

	m_sceneNode = new Node(m_logicalDevice,m_swapChain, m_descriptorSetLayout);

	m_swapChain->setRenderPass(m_renderPass);
	m_swapChain->createFrameBuffers();
}

Application::~Application()
{
	delete m_sceneNode;
	delete m_descriptorSetLayout;
	delete m_pipeline;
	delete m_renderPass;
	delete m_swapChain;
	delete m_logicalDevice;
	delete m_physicalDevice;
	delete m_surface;
	delete m_debugUtilsMessenger;
	delete m_instance;
	delete m_mainWindow;
}

int Application::exec()
{
	while (!m_mainWindow->shouldClose()) {
		m_mainWindow->pollEvents();
		m_sceneNode->drawFrame(m_renderPass,m_pipeline);
	}

	m_logicalDevice->Wait();

	return 0;
}

bool Application::debug() const 
{
	return m_debug;
}

const std::vector<const char*>& Application::getInstanceExtensions() const
{
	return m_instanceExtensions;
}

const std::vector<const char*>& Application::getDeviceExtensions() const
{
	return m_deviceExtensions;
}

const std::vector<const char*>& Application::getInstanceLayers() const
{
	return m_instanceLayers;
}

const std::string_view& Application::getName() const
{
	return m_name;
}

void Application::onEvent(Event& event)
{

	//std::cout << "Height :" << event.windowHeight << ", width :" << event.windowWidth << std::endl;

	if (event.windowHeight == 0 && event.windowWidth == 0)
	{
		int width = 0, height = 0;
		while (width == 0 || height == 0) {
			m_mainWindow->getWindowSize(width,height);
			m_mainWindow->waitEvents();
		}

		m_logicalDevice->Wait();
		return;

	}

	m_logicalDevice->Wait();
	m_swapChain->cleanup();
	m_swapChain->create();
	m_swapChain->createImageViews();
	m_swapChain->createFrameBuffers();
}

Application* Application::instance()
{
	return g_instance;
}
