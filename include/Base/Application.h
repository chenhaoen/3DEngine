#pragma once

#include <string_view>
#include <vector>

#include "Base/Exports.h"

class Window;
class Event;
class FrameManager;
class LayerManager;
class Context;
class Node;
class BASE_API Application
{
public:
	Application(const std::string_view &name, const bool debug);
	~Application();

	int exec();

	bool debug() const;

	const std::string_view &getName() const;

	void onEvent(Event &event);

	static constexpr int maxFrameCount() { return 2; };

	static Application *instance();

	void setModelFile(const std::string_view&);
private:
	Window *m_mainWindow;
	FrameManager *m_frameManager;
	LayerManager *m_layerManager;
	Context* m_context;

	bool m_debug;
	std::string_view m_name;

	static Application *g_instance;
};