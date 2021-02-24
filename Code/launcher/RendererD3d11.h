#pragma once

#include <wrl.h>
#include <d3d11_1.h>
#include <memory>
#include <string>
#include <TiltedCore/Stl.hpp>

#include <UIApp.hpp>
#include <UIViewD3D11.hpp>

// im just lazy, lol
template<typename T>
using CmPtr = Microsoft::WRL::ComPtr<T>;

using namespace TiltedPhoques;

struct D3D11RenderProvider;
class Window;

class RendererD3d11 final : public UIViewD3D11::Renderer
{
    friend struct D3D11RenderProvider;
public:
    enum class Result
    {
        kSuccess,
        kDxgiFactoryErr,
        kNoSuitableDevice,
        kSwapchainError,
        kRTVError,
    };

    RendererD3d11();
    ~RendererD3d11();

    Result Create(const Window&) noexcept;

    void BeginDraw();
    void EndDraw();

    void Resize(int aNewWidth, int aNewHeight);

    IDXGISwapChain* GetSwapChain() const noexcept override;
    ID3D11Device* GetDevice() noexcept;
    ID3D11DeviceContext* GetD3D11DeviceContext() noexcept;

  private:
    Result CreateRenderTarget(int aWidth, int aHeight);

    CmPtr<ID3D11Device> m_pDevice;
    CmPtr<ID3D11DeviceContext> m_pDevCtx;
    CmPtr<IDXGISwapChain> m_pSwapchain;
    CmPtr<ID3D11RenderTargetView> m_pRtv;
};
