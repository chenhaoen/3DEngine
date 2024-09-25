#pragma once 

#include <string_view>
#include <vector>

#include <vulkan/vulkan.h>

class Instance;
class Window
{
public:
	Window(unsigned int width, unsigned int height, const std::string_view& title);
	~Window();

	bool shouldClose() const;

	void pollEvents() const;

	void waitEvents() const;

	VkSurfaceKHR createSurface(Instance* instance);
	void getFrameBufferSize(int& width, int& height);
	void getWindowSize(int& width, int& height);

	void* naviteWindow() const;

	static std::vector<const char*> GetRequiredInstanceExtensions();
private:
	void* m_pWindow;
};