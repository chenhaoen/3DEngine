#include "Vk/Surface.h"
#include "Vk/Instance.h"

#include "Base/Window.h"

Surface::Surface(Instance* instance, Window* window)
	:m_instance(instance)
{
	m_vkSurface = window->createSurface(instance);
}

Surface::~Surface()
{
	vkDestroySurfaceKHR(m_instance->getVkInstance(), m_vkSurface, nullptr);
}

VkSurfaceKHR Surface::getVkSurface() const
{
	return m_vkSurface;
}
