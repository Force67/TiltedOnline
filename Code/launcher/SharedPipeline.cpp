
#include <d3d11.h>
#include <memory>

#include "SharedWindow.h"
#include "SharedPipeline.h"

constexpr FLOAT kClearColor_Red[4] = {1.f, 0.270588249f, 0.f, 1.f};

SharedPipeline::SharedPipeline() = default;
SharedPipeline::~SharedPipeline() = default;

SharedPipeline::Result SharedPipeline::Create(const SharedWindow& aWindow)
{
    auto cViewportSize = aWindow.GetWindowRect();

    CmPtr<IDXGIFactory> pFactory = nullptr;
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.ReleaseAndGetAddressOf()));
    if (FAILED(hr) || !pFactory)
        return Result::kDeviceFactoryError;

    CmPtr<IDXGIAdapter> pAdapter = nullptr;
    pFactory->EnumAdapters(0, &pAdapter);
    if (!pAdapter)
        return Result::kDeviceNotFound;

    // its 0 by default in Bethesda viewport class
    CmPtr<IDXGIOutput> pOutPut = nullptr;
    pAdapter->EnumOutputs(0, pOutPut.GetAddressOf());
    if (!pOutPut)
        return Result::kDeviceNotFound;

    UINT numModes = 0;
    std::unique_ptr<DXGI_MODE_DESC[]> modes;
    do
    {
        hr = pOutPut->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
        if (FAILED(hr))
            return Result::kDeviceNotFound;

        modes = std::make_unique<DXGI_MODE_DESC[]>(numModes);
        hr =
            pOutPut->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, modes.get());

    } while (hr == DXGI_ERROR_MORE_DATA);

    DXGI_SWAP_CHAIN_DESC desc{};
    bool modeFound = false;

    /*
    for (UINT i = 0; i < numModes; ++i)
    {
        const auto& mode = modes[i];
        if (mode.Width == cWidth && mode.Height == cHeight)
        {
            if (mode.Scaling)
            {
                desc.BufferDesc.Scaling = mode.Scaling;
            }
            if (!modeFound)
            {
                modeFound = true;
                desc.BufferDesc.RefreshRate = mode.RefreshRate;
            }
            else
            {
                if ((mode.RefreshRate.Numerator / mode.RefreshRate.Denominator) >= (60 / 1))
                    continue;
                desc.BufferDesc.RefreshRate = mode.RefreshRate;
            }
        }
    }*/

    pOutPut.Reset();

    desc.BufferCount = 2;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    desc.BufferDesc.Width = cViewportSize.x;
    desc.BufferDesc.Height = cViewportSize.y;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
    desc.OutputWindow = aWindow.GetNativeHandle();
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Windowed = TRUE;

    // we use D3D_FEATURE_LEVEL_11_0 instead of 9_1 for better CEF drawing support
    auto featureLevel = D3D_FEATURE_LEVEL_11_0;
    auto createFlags = 0;

    // if (bUseDeviceDebug)
    //    createFlags = D3D11_CREATE_DEVICE_DEBUG;

    // for now we create the device with vsync
    hr = D3D11CreateDeviceAndSwapChain(pAdapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, createFlags, nullptr, 0,
                                       D3D11_SDK_VERSION, &desc, m_pSwapchain.ReleaseAndGetAddressOf(),
                                       m_pDevice.ReleaseAndGetAddressOf(), &featureLevel,
                                       m_pDevCtx.ReleaseAndGetAddressOf());

    if (FAILED(hr))
        return Result::kDeviceCreateError;

    //  auto res = CreateRenderTarget(aWidth, aHeight);
    //  if (res != Result::kSuccess)
    //      return res;

    // todo: get rid of DXGI_MWA_NO_WINDOW_CHANGES  call
    // rebuild ui on game kill...
    return Result::kSuccess;
}

void SharedPipeline::Resize(glm::uvec2 aSize)
{
    // clear render target
    m_pDevCtx->OMSetRenderTargets(0, 0, nullptr);
    m_pRtv.Reset();

    HRESULT hr = m_pSwapchain->ResizeBuffers(0, aSize.x, aSize.y, DXGI_FORMAT_R8G8B8A8_UNORM, 2);
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
    vp.Width = static_cast<float>(aSize.x);
    vp.Height = static_cast<float>(aSize.y);
    vp.MinDepth = D3D11_MIN_DEPTH;
    vp.MaxDepth = D3D11_MAX_DEPTH;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pDevCtx->RSSetViewports(1, &vp);
}

void SharedPipeline::BeginDraw()
{
    // bind the current view
    m_pDevCtx->OMSetRenderTargets(1, m_pRtv.GetAddressOf(), nullptr);

    // clear the screen
    m_pDevCtx->ClearRenderTargetView(m_pRtv.Get(), kClearColor_Red);
}

void SharedPipeline::EndDraw()
{
    m_pSwapchain->Present(1, 0);
}
