
#include "Window.h"
#include <OverlayApp.hpp>
#include <FunctionHook.hpp>
#include <TiltedCore/Initializer.hpp>

static Window* g_windowInstance = nullptr;
static WNDPROC g_gameWndProc = nullptr;

Window::Window(MessageHandler& aHandler) : m_Handler(aHandler)
{
    g_windowInstance = this;
}

Window::~Window()
{
    g_windowInstance = nullptr;
}

bool Window::Create(const wchar_t* aWindowName, int32_t aWidth /* = 0 */, int32_t aHeight /* = 0 */)
{
    WNDCLASSW wc{};
    wc.style = 3;
    wc.lpfnWndProc = StaticWndProc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.hIcon = LoadIconW(wc.hInstance, MAKEINTRESOURCEW(102) /*TaskBar icon*/);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(4));
    wc.lpszClassName = L"TOWindowClass";
    if (!RegisterClassW(&wc))
        return false;

    // TODO: determine style based on config
    //const DWORD windowStyle = 0x80000000; // 0xC00000
    const DWORD windowStyle = 0xC0000;
    const DWORD exStyle = 0;

    if (aWidth == 0 && aHeight == 0)
    {
        GetOptimalResolution(aWidth, aHeight);
    }

    RECT rc{};
    SetRect(&rc, 0, 0, aWidth, aHeight);
    AdjustWindowRect(&rc, windowStyle, 0);

    int32_t width = rc.right - rc.left;
    int32_t height = rc.bottom - rc.top;

    m_pHwnd = CreateWindowExW(exStyle, wc.lpszClassName, aWindowName, windowStyle, CW_USEDEFAULT, 0, width, height,
                              nullptr, nullptr, wc.hInstance, this);

    return m_pHwnd;
}

void Window::Show()
{
    if (m_pHwnd)
    {
        ShowWindow(m_pHwnd, 5);
        SetForegroundWindow(m_pHwnd);
        SetFocus(m_pHwnd);
    }
}

void Window::PollEvents()
{
    MSG msg;
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            m_bShouldQuit = true;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
}

void Window::HandleMessage(UINT aMsg, WPARAM aWp, LPARAM aLp)
{
    if (aMsg == WM_CLOSE)
        m_bShouldQuit = true;
    
    EventType type = EventType::kNone;
    switch (aMsg)
    {
    case WM_SIZE:
        type = EventType::kResized;
        break;
    default:
        break;
    }

    if (type != EventType::kNone)
    {
        m_Handler.HandleMessage(type);
    }
}

void Window::GetOptimalResolution(int32_t& x, int32_t& y)
{
    x = GetSystemMetrics(SM_CXSCREEN);
    y = GetSystemMetrics(SM_CYSCREEN);
}

bool Window::ShouldClose() const
{
    return m_bShouldQuit;
}

const HWND Window::GetNativeHandle() const 
{
    return m_pHwnd;
}

Window::WindowDimensions Window::GetWindowDimensions() const
{
    RECT rect;
    GetClientRect(m_pHwnd, &rect);

    return {
        static_cast<int32_t>(rect.right - rect.left), 
        static_cast<int32_t>(rect.bottom - rect.top)};
}

LRESULT CALLBACK Window::StaticWndProc(HWND aHwnd, UINT aMsg, WPARAM aWp, LPARAM aLp)
{
    if (g_gameWndProc)
    {
        static bool x = false;
        if (!x)
        {
            
        }

        g_gameWndProc(aHwnd, aMsg, aWp, aLp);
    }

    Window* pPtr = reinterpret_cast<Window*>(GetWindowLongPtr(aHwnd, GWLP_USERDATA));
    if (aMsg == WM_NCCREATE)
    {
        // set user pointer
        if (!pPtr)
        {
            pPtr = reinterpret_cast<Window*>(reinterpret_cast<CREATESTRUCT*>(aLp)->lpCreateParams);

            SetWindowLongPtrW(aHwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pPtr));
        }
    }

    pPtr->HandleMessage(aMsg, aWp, aLp);
    return DefWindowProcW(aHwnd, aMsg, aWp, aLp);
}

static HWND CreateWindowExA_Hook(DWORD dwExStyle, 
    const char* lpClassName, 
    const char* lpWindowName, 
    DWORD dwStyle, int X, int Y,
    int nWidth, int nHeight, 
    HWND hWndParent, HMENU hMenu, 
    HINSTANCE hInstance, LPVOID lpParam)
{

    HWND handle = g_windowInstance->GetNativeHandle();
    //SetWindowPos(handle, nullptr, X, Y, 0, 0, 0);
    //SetWindowLongPtr(handle, GWL_STYLE, dwStyle);

    //auto newText = fmt::format();
    //SetWindowTextW();

    return handle;
}

ATOM RegisterClassA_Hook(const WNDCLASSA* lpWndClass)
{
    g_gameWndProc = lpWndClass->lpfnWndProc;

    g_gameWndProc(g_windowInstance->GetNativeHandle(), WM_ACTIVATE, WA_ACTIVE, 0);
    return 0;
}

LRESULT DefWindowProcA_Hook(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProcW(hWnd, Msg, wParam, lParam);
}

static TiltedPhoques::Initializer init([] 
{ 
   // TiltedPhoques::HookIATIm(nullptr, "User32.dll", "RegisterClassA", RegisterClassA_Hook);
   // TiltedPhoques::HookIATIm(nullptr, "User32.dll", "CreateWindowExA", CreateWindowExA_Hook);
   // TiltedPhoques::HookIATIm(nullptr, "User32.dll", "DefWindowProcA", DefWindowProcA_Hook);
});
