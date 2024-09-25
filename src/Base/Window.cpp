#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Base/Window.h"
#include "Base/Instance.h"
#include "Base/Surface.h"
#include "Base/Application.h"
#include "Base/Event.h"

namespace
{
    void resizeCallBack(GLFWwindow *window, int width, int height)
    {
        Event event;
        event.windowHeight = height;
        event.windowWidth = width;
        Application::instance()->onEvent(event);
    }

    void glfw_error_callback(int error, const char *description)
    {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    }
}

Window::Window(unsigned int width, unsigned int height, const std::string_view &title)
{
    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_pWindow = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

    glfwSetWindowSizeCallback(static_cast<GLFWwindow *>(m_pWindow), resizeCallBack);
}

Window::~Window()
{
    glfwDestroyWindow(static_cast<GLFWwindow *>(m_pWindow));

    glfwTerminate();
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(static_cast<GLFWwindow *>(m_pWindow));
}

void Window::pollEvents() const
{
    glfwPollEvents();
}

void Window::waitEvents() const
{
    glfwWaitEvents();
}

VkSurfaceKHR Window::createSurface(Instance *instance)
{
    VkSurfaceKHR vkSurface;

    if (glfwCreateWindowSurface(instance->getVkInstance(), static_cast<GLFWwindow *>(m_pWindow), nullptr, &vkSurface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }

    return vkSurface;
}

void Window::getFrameBufferSize(int &width, int &height)
{
    glfwGetFramebufferSize(static_cast<GLFWwindow *>(m_pWindow), &width, &height);
}

void Window::getWindowSize(int &width, int &height)
{
    glfwGetWindowSize(static_cast<GLFWwindow *>(m_pWindow), &width, &height);
}

void* Window::naviteWindow() const
{
    return m_pWindow;
}

std::vector<const char *> Window::GetRequiredInstanceExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions = nullptr;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    return {glfwExtensions, glfwExtensions + glfwExtensionCount};
}
