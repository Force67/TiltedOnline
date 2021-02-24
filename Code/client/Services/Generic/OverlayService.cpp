#include <TiltedOnlinePCH.h>

#include <Services/OverlayService.h>

#include <UIApp.hpp>
#include <UIViewD3D11.hpp>

#include <TiltedHooks/Code/hooks/include/D3D11Hook.hpp>
#include <Systems/RenderSystemD3D11.h>

OverlayService::OverlayService()
{
    m_pOverlay = new TiltedPhoques::UIApp();
}

OverlayService::~OverlayService() noexcept
{
}

void OverlayService::Create(RenderSystemD3D11* apRenderSystem)
{
    using namespace TiltedPhoques;

    TiltedPhoques::OverlayCreateInfo info;
    info.highDPI = true;
    info.processName = L"TPProcess.exe";
    m_pOverlay->Initialize(info);

    //m_pTestframe = new UIViewD3D11(apRenderSystem->GetSwapChain());
    //m_pTestframe->OpenUrl("https://google.de", apRenderSystem->GetWindow());
}

void OverlayService::Render() const
{
    m_pOverlay->DrawFrames();
}

void OverlayService::Reset() const
{
    //m_pOverlay->GetClient()->Reset();
}
