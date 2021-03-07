
#include <d3d11.h>
#include <FunctionHook.hpp>
#include <TiltedCore/Initializer.hpp>

#include "SharedWindow.h"
#include "SharedPipeline.h"

#include "Launcher.h"

static WNDPROC g_pChildWndProc = nullptr;

HRESULT TP_D3D11CreateDeviceAndSwapChain(
    IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software,
    UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels,
    UINT SDKVersion, const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
    IDXGISwapChain** ppSwapChain, ID3D11Device** ppDevice,
    D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext)
{
    *pFeatureLevel = D3D_FEATURE_LEVEL_11_0;

    const auto& pipeline = GetLauncher()->GetPipeline();

    *ppDevice = pipeline.GetDevice();
    *ppSwapChain = pipeline.GetSwapChain();
    *ppImmediateContext = pipeline.GetContext();

    // adjust pipeline to game format
    (*ppSwapChain)->ResizeBuffers(
        pSwapChainDesc->BufferCount, pSwapChainDesc->BufferDesc.Width,
        pSwapChainDesc->BufferDesc.Height, pSwapChainDesc->BufferDesc.Format, 
        pSwapChainDesc->Flags);

    DXGI_MODE_DESC md{};
    md.Format = pSwapChainDesc->BufferDesc.Format;
    md.Height = pSwapChainDesc->BufferDesc.Height;
    md.Width = pSwapChainDesc->BufferDesc.Width;
    md.RefreshRate = pSwapChainDesc->BufferDesc.RefreshRate;
    md.Scaling = pSwapChainDesc->BufferDesc.Scaling;
    md.ScanlineOrdering = pSwapChainDesc->BufferDesc.ScanlineOrdering;
    (*ppSwapChain)->ResizeTarget(&md);

    return 0; // OK
}

static HWND TP_CreateWindowExA(
    DWORD dwExStyle, const char* lpClassName, 
    const char* lpWindowName, DWORD dwStyle,
    int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,
    HINSTANCE hInstance, LPVOID lpParam)
{
    HWND windowHandle = GetLauncher()->GetWindow().GetNativeHandle();

    // send a fake activate message to make the game aware
    g_pChildWndProc(windowHandle, WM_ACTIVATE, WA_ACTIVE, 0);

    return windowHandle;
}

ATOM TP_RegisterClassA(const WNDCLASSA* lpWndClass)
{
    g_pChildWndProc = lpWndClass->lpfnWndProc;

    GetLauncher()->GetWindow().
        SetChildWndProc(lpWndClass->lpfnWndProc);
    return 0;
}

LRESULT TP_DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    // the game uses CreateWindowExA but we use CreateWindowExW so we must redefine
    // the message loop to support wide content
    return DefWindowProcW(hWnd, Msg, wParam, lParam);
}

BOOL TP_CloseHandle(HANDLE apHandle)
{
    auto& window = GetLauncher()->GetWindow();
    if (apHandle == window.GetNativeHandle())
    {
        window.SetChildWndProc(nullptr);
        return TRUE;
    }

    return CloseHandle(apHandle);
}

static TiltedPhoques::Initializer s_init([] {
    TP_HOOK_IAT2("User32.dll", "RegisterClassA", TP_RegisterClassA);
    TP_HOOK_IAT2("User32.dll", "CreateWindowExA", TP_CreateWindowExA);
    TP_HOOK_IAT2("User32.dll", "DefWindowProcA", TP_DefWindowProcA);
    TP_HOOK_IAT2("Kernel32.dll", "CloseHandle", TP_CloseHandle);
    TP_HOOK_IAT2("d3d11.dll", "D3D11CreateDeviceAndSwapChain", TP_D3D11CreateDeviceAndSwapChain);
});
