
#include "RendererD3d11.h"

// adapter for our render model
struct D3D11RenderProvider : OverlayApp::RenderProvider
{
    explicit D3D11RenderProvider(RendererD3d11* aRenderer) : m_pRenderer(aRenderer)
    {}

    OverlayRenderHandler* Create() override
    {
        auto* pRenderHandler = new OverlayRenderHandlerD3D11(m_pRenderer);
        // configure the render handler to be visible by default
        // for the launcher
        pRenderHandler->SetVisible(true);

        return pRenderHandler;
    }

    [[nodiscard]] HWND GetWindow() override
    {
        return m_pRenderer->m_pHwnd;
    }

  private:
    RendererD3d11* m_pRenderer;
};

static RendererD3d11* g_Renderer = nullptr;

RendererD3d11::RendererD3d11()
{
    g_Renderer = this;
}

RendererD3d11::~RendererD3d11()
{
    g_Renderer = nullptr;
}

RendererD3d11::Result RendererD3d11::Create(HWND aHwnd, int aWidth, int aHeight) noexcept
{
    CmPtr<IDXGIFactory> pFactory = nullptr;
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.ReleaseAndGetAddressOf()));
    if (FAILED(hr) || !pFactory)
        return Result::kDxgiFactoryErr;

    CmPtr<IDXGIAdapter> pAdapter = nullptr;
    pFactory->EnumAdapters(0, &pAdapter);
    if (!pAdapter)
        return Result::kNoSuitableDevice;

    /*IDXGIOutput* pOutput = nullptr;
    pAdapter->EnumOutputs(0, &pOutput);
    if (!pOutput)
        return Result::kNoSuitableDevice;

    // this is not recommended to be done, but we are basing out implementation
    // on the Bethesda renderer
    UINT numModes = 0;
    pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);*/

    DXGI_SWAP_CHAIN_DESC desc{};
    desc.BufferCount = 2;
    desc.Flags = 2;
    desc.BufferDesc.Width = aWidth;
    desc.BufferDesc.Height = aHeight;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferUsage = 48;
    desc.OutputWindow = aHwnd;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Windowed = TRUE;

    // yep...
    // betheseda uses: D3D_FEATURE_LEVEL_9_1, but we need to use D3D_FEATURE_LEVEL_11_0
    // for CEF compatability
    auto featureLevel = D3D_FEATURE_LEVEL_11_0;

    // for now we create the device with vsync
    hr = D3D11CreateDeviceAndSwapChain(pAdapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, 2, nullptr, 0, 7u, &desc,
                                  m_pSwapchain.ReleaseAndGetAddressOf(), m_pDevice.ReleaseAndGetAddressOf(),
                                  &featureLevel, m_pDevCtx.ReleaseAndGetAddressOf());

    if (FAILED(hr))
        return Result::kNoSuitableDevice;

    m_pHwnd = aHwnd;

    auto res = CreateRenderTarget(aWidth, aHeight);
    if (res != Result::kSuccess)
        return res;

    // todo: get rid of DXGI_MWA_NO_WINDOW_CHANGES  call
    // rebuild ui on game kill...
    return Result::kSuccess;
}

RendererD3d11::Result RendererD3d11::CreateRenderTarget(int aWidth, int aHeight)
{
    CmPtr<ID3D11Texture2D> pBackBuffer = nullptr;
    HRESULT hr = m_pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(pBackBuffer.ReleaseAndGetAddressOf()));
    if (FAILED(hr))
        return Result::kSwapchainError;

    D3D11_TEXTURE2D_DESC bbDesc{};
    pBackBuffer->GetDesc(&bbDesc);
    //debug

    hr = m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, m_pRtv.ReleaseAndGetAddressOf());
    if (FAILED(hr))
    {
        return Result::kRTVError;
    }

    m_pDevCtx->OMSetRenderTargets(1, m_pRtv.GetAddressOf(), nullptr);

    D3D11_VIEWPORT vp;
    vp.Width = static_cast<float>(aWidth);
    vp.Height = static_cast<float>(aHeight);
    vp.MinDepth = D3D11_MIN_DEPTH;
    vp.MaxDepth = D3D11_MAX_DEPTH;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pDevCtx->RSSetViewports(1, &vp);

    return Result::kSuccess;
}

std::unique_ptr<OverlayApp::RenderProvider> RendererD3d11::CreateRenderProvider()
{
    return std::unique_ptr<OverlayApp::RenderProvider>(new D3D11RenderProvider(this));
}

void RendererD3d11::Resize(int aNewWidth, int aNewHeight)
{
    // clear render target
    m_pDevCtx->OMSetRenderTargets(0, 0, nullptr);
    m_pRtv.Reset();

    HRESULT hr = m_pSwapchain->ResizeBuffers(0, 
        static_cast<UINT>(aNewWidth), static_cast<UINT>(aNewHeight),
                                DXGI_FORMAT_R8G8B8A8_UNORM, 2);

    if (FAILED(hr))
        return;

    CmPtr<ID3D11Texture2D> pBackBuffer = nullptr;
    hr = m_pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                 reinterpret_cast<void**>(pBackBuffer.ReleaseAndGetAddressOf()));
    if (FAILED(hr))
        return;

    hr = m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, m_pRtv.ReleaseAndGetAddressOf());
    if (FAILED(hr))
        return;

    m_pDevCtx->OMSetRenderTargets(1, m_pRtv.GetAddressOf(), nullptr);

    D3D11_VIEWPORT vp;
    vp.Width = static_cast<float>(aNewWidth);
    vp.Height = static_cast<float>(aNewHeight);
    vp.MinDepth = D3D11_MIN_DEPTH;
    vp.MaxDepth = D3D11_MAX_DEPTH;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pDevCtx->RSSetViewports(1, &vp);
}

void RendererD3d11::Begin()
{
    // bind the current view
    m_pDevCtx->OMSetRenderTargets(1, m_pRtv.GetAddressOf(), nullptr);
    
    // clear the screen
    FLOAT color[4] = {1.f, 0.270588249f, 0.f, 1.f};
    m_pDevCtx->ClearRenderTargetView(m_pRtv.Get(), color);
    // bind the swapchain
}

// submit frame
void RendererD3d11::End()
{
    m_pSwapchain->Present(1, 0);
}

IDXGISwapChain* RendererD3d11::GetSwapChain() const noexcept
{
    return m_pSwapchain.Get();
}

// TODO: hook event responder: -> hooks IAT
// via TiltedPhoques:: Intiailizer