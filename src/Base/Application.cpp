#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <memory>

#include "Base/Application.h"
#include "Base/Window.h"
#include "Base/Event.h"
#include "Base/FrameManager.h"
#include "Base/LayerManager.h"
#include "Base/Context.h"
#include "Base/LogicalDevice.h"
#include "Base/SwapChain.h"

Application *Application::g_instance = nullptr;

Application::Application(
	const std::string_view &name,
	const bool debug)
	: m_name(name),
	  m_debug(debug)
{
	if (g_instance != nullptr)
	{
		throw std::runtime_error("Application already exists!");
	}

	g_instance = this;

	m_mainWindow = new Window(1600, 1200, m_name);

	m_context = new Context();
	m_context->setWindow(m_mainWindow);
	m_context->init();

	m_frameManager = new FrameManager();
	m_layerManager = new LayerManager(m_mainWindow);
}

Application::~Application()
{
	delete m_layerManager;
	delete m_frameManager;
	delete m_context;
	delete m_mainWindow;
}

int Application::exec()
{
	while (!m_mainWindow->shouldClose())
	{
		m_mainWindow->pollEvents();

		m_frameManager->frame(m_layerManager);
	}

	m_context->getDevice()->Wait();

	return 0;
}

bool Application::debug() const
{
	return m_debug;
}

const std::string_view &Application::getName() const
{
	return m_name;
}

void Application::onEvent(Event &event)
{

	// std::cout << "Height :" << event.windowHeight << ", width :" << event.windowWidth << std::endl;

	if (event.windowHeight == 0 && event.windowWidth == 0)
	{
		int width = 0, height = 0;
		while (width == 0 || height == 0)
		{
			m_mainWindow->getWindowSize(width, height);
			m_mainWindow->waitEvents();
		}

		m_context->getDevice()->Wait();
		return;
	}

	m_context->getDevice()->Wait();
	m_context->getSwapChain()->cleanup();
	m_context->getSwapChain()->create();
	m_context->getSwapChain()->createImageViews();
	m_context->getSwapChain()->createFrameBuffers();
}

Application *Application::instance()
{
	return g_instance;
}
