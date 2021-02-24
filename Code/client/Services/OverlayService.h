#pragma once

#include <include/internal/cef_ptr.h>

namespace TiltedPhoques
{
    class UIApp;
class UIView;
}

struct RenderSystemD3D11;

struct OverlayService
{
    OverlayService();
    ~OverlayService() noexcept;

    TP_NOCOPYMOVE(OverlayService);

    void Create(RenderSystemD3D11* apRenderSystem);

    void Render() const;
    void Reset() const;

    TiltedPhoques::UIApp* GetOverlayApp() const noexcept { return m_pOverlay.get(); }

private:

    CefRefPtr<TiltedPhoques::UIApp> m_pOverlay{nullptr};
    CefRefPtr<TiltedPhoques::UIView> m_pTestframe = nullptr;
};
