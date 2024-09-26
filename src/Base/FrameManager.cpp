#include <algorithm>
#include <memory>

#include "Base/FrameManager.h"
#include "Base/Layer.h"
#include "Base/Application.h"
#include "Base/CommandPool.h"
#include "Base/Frame.h"
#include "Base/LayerManager.h"

FrameManager::FrameManager(LogicalDevice *device, SwapChain *swapChain, RenderPass *renderPass)
    : m_currentFrame(0), m_device(device), m_swapChain(swapChain), m_renderPass(renderPass)
{
    m_commandPool = new CommandPool(device);

    for (uint32_t i = 0; i < Application::maxFrameCount(); ++i)
    {
        m_frames.push_back(new Frame(m_device, m_commandPool, swapChain, m_renderPass));
    }
}

FrameManager::~FrameManager()
{
    delete m_commandPool;
    std::for_each(m_frames.begin(), m_frames.end(), std::default_delete<Frame>());
}

void FrameManager::frame(LayerManager *layerManager)
{
    Frame *currentFrame = m_frames[m_currentFrame];
    currentFrame->begin();
    layerManager->recordCommandBuffer(currentFrame);
    currentFrame->end();

    m_currentFrame = (m_currentFrame + 1) % Application::maxFrameCount();
}