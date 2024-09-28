#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

#include "Base/OverlayLayer.h"
#include "Base/Frame.h"
#include "Base/Window.h"
#include "Base/LogicalDevice.h"
#include "Base/PhysicalDevice.h"
#include "Base/Instance.h"
#include "Base/RenderPass.h"
#include "Base/Context.h"

OverlayLayer::OverlayLayer(Window *window)
	: Layer(window)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

	VkDescriptorPoolSize pool_sizes[] =
		{
			{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1},
		};
	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1;
	pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;
	vkCreateDescriptorPool(Context::instance()->getDevice()->getVkDevice(), &pool_info, nullptr, &m_descriptorPool);

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForVulkan((GLFWwindow *)(m_window->naviteWindow()), true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = Context::instance()->getInsance()->getVkInstance();
	init_info.PhysicalDevice = Context::instance()->getDevice()->getPhysicalDevice()->getVkPhysicalDevice();
	init_info.Device = Context::instance()->getDevice()->getVkDevice();
	init_info.QueueFamily = Context::instance()->getDevice()->getPhysicalDevice()->getGraphicsFamilyIndex();
	init_info.Queue = Context::instance()->getDevice()->getGraphicsQueue();
	init_info.PipelineCache = nullptr;
	init_info.DescriptorPool = m_descriptorPool;
	init_info.RenderPass = Context::instance()->getRenderPass()->getVkRenderPass();
	init_info.Subpass = 0;
	init_info.MinImageCount = 2;
	init_info.ImageCount = 3;
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	init_info.Allocator = nullptr;
	init_info.CheckVkResultFn = nullptr;
	ImGui_ImplVulkan_Init(&init_info);
}

OverlayLayer::~OverlayLayer()
{
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	vkDestroyDescriptorPool(Context::instance()->getDevice()->getVkDevice(), m_descriptorPool, nullptr);
}

// Our state
bool show_demo_window = false;
bool show = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void OverlayLayer::recordCommandBuffer(Frame *frame)
{
	// Start the Dear ImGui frame
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		//ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{

				if (ImGui::MenuItem("Open"))
				{
				}

				if (ImGui::MenuItem("Exit"))
				{
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z"))
				{
				}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
				{
				} // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X"))
				{
				}
				if (ImGui::MenuItem("Copy", "CTRL+C"))
				{
				}
				if (ImGui::MenuItem("Paste", "CTRL+V"))
				{
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// ImGui::Text("This is some useful text.");		   // Display some text (you can use a format strings too)
		// ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
		// ImGui::Checkbox("Another Window", &show_another_window);

		// ImGui::SliderFloat("float", &f, 0.0f, 1.0f);			 // Edit 1 float using a slider from 0.0f to 1.0f
		// ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

		// if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
		// 	counter++;
		// ImGui::SameLine();
		// ImGui::Text("counter = %d", counter);

		// ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		//ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	ImDrawData *draw_data = ImGui::GetDrawData();
	const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

	VkClearValue clearValue = frame->getClearVaule();
	clearValue.color = {clear_color.x, clear_color.y, clear_color.z, clear_color.w};
	frame->setClearValue(clearValue);

	ImGui_ImplVulkan_RenderDrawData(draw_data, frame->getCommandBuffer());
}