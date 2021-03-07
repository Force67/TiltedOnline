#pragma once

#include <wrl.h>
#include <glm/glm.hpp>

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

template <typename T> using CmPtr = Microsoft::WRL::ComPtr<T>;

class SharedWindow;

class SharedPipeline
{
public:
    enum class Result
    {
        kSuccess,
        kDeviceFactoryError,
        kDeviceCreateError,
        kDeviceNotFound,
        kSwapchainError,
        kRenderError,
    };

    SharedPipeline();
    ~SharedPipeline();

    Result Create(const SharedWindow& aWindow);

    void Resize(glm::uvec2 aViewportSize);

    void BeginDraw();
    void EndDraw();

    inline IDXGISwapChain* GetSwapChain() const { return m_pSwapchain.Get(); }
    inline ID3D11DeviceContext* GetContext() const { return m_pDevCtx.Get(); }
    inline ID3D11Device* GetDevice() const { return m_pDevice.Get(); }

private:
    CmPtr<ID3D11Device> m_pDevice;
    CmPtr<ID3D11DeviceContext> m_pDevCtx;
    CmPtr<IDXGISwapChain> m_pSwapchain;
    CmPtr<ID3D11RenderTargetView> m_pRtv;
};
