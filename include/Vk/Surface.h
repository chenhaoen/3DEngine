#pragma once

#include <vulkan/vulkan.h>

class Window;
class Instance;
class Surface
{
public:
	Surface(Instance* instance, Window* window);
	~Surface();

	VkSurfaceKHR getVkSurface() const;
private:
	VkSurfaceKHR m_vkSurface;
	Instance* m_instance;
};

