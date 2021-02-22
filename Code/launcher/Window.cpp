
#include "Window.h"
#include <OverlayApp.hpp>
#include <FunctionHook.hpp>
#include <TiltedOnlineApp.h>
#include <TiltedCore/Initializer.hpp>

static Window* g_windowInstance = nullptr;
static WNDPROC g_gameWndProc = nullptr;

static TiltedPhoques::OverlayApp* s_pApp = nullptr;

void WINDOWHACK_INIT(TiltedPhoques::OverlayApp * pApp)
{
    s_pApp = pApp;
}

// HACK!
namespace
{

uint32_t GetCefModifiers(uint16_t aVirtualKey)
{
    uint32_t modifiers = EVENTFLAG_NONE;

    if (GetKeyState(VK_MENU) & 0x8000)
    {
        modifiers |= EVENTFLAG_ALT_DOWN;
    }

    if (GetKeyState(VK_CONTROL) & 0x8000)
    {
        modifiers |= EVENTFLAG_CONTROL_DOWN;
    }

    if (GetKeyState(VK_SHIFT) & 0x8000)
    {
        modifiers |= EVENTFLAG_SHIFT_DOWN;
    }

    if (GetKeyState(VK_LBUTTON) & 0x8000)
    {
        modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
    }

    if (GetKeyState(VK_RBUTTON) & 0x8000)
    {
        modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
    }

    if (GetKeyState(VK_MBUTTON) & 0x8000)
    {
        modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
    }

    if (GetKeyState(VK_CAPITAL) & 1)
    {
        modifiers |= EVENTFLAG_CAPS_LOCK_ON;
    }

    if (GetKeyState(VK_NUMLOCK) & 1)
    {
        modifiers |= EVENTFLAG_NUM_LOCK_ON;
    }

    if (aVirtualKey)
    {
        if (aVirtualKey == VK_RCONTROL || aVirtualKey == VK_RMENU || aVirtualKey == VK_RSHIFT)
        {
            modifiers |= EVENTFLAG_IS_RIGHT;
        }
        else if (aVirtualKey == VK_LCONTROL || aVirtualKey == VK_LMENU || aVirtualKey == VK_LSHIFT)
        {
            modifiers |= EVENTFLAG_IS_LEFT;
        }
        else if (aVirtualKey >= VK_NUMPAD0 && aVirtualKey <= VK_DIVIDE)
        {
            modifiers |= EVENTFLAG_IS_KEY_PAD;
        }
    }

    return modifiers;
}

void ProcessKeyboard(uint16_t aKey, uint16_t aScanCode, cef_key_event_type_t aType, bool aE0, bool aE1)
{
    if (aType == KEYEVENT_KEYDOWN && aKey == VK_RCONTROL)
    {
        //ShowCursor(!active);
    }
}

void ProcessMouseMove(uint16_t aX, uint16_t aY)
{
    s_pApp->InjectMouseMove(aX, aY, GetCefModifiers(0));
}

void ProcessMouseButton(uint16_t aX, uint16_t aY, cef_mouse_button_type_t aButton, bool aDown)
{
    s_pApp->InjectMouseButton(aX, aY, aButton, !aDown, GetCefModifiers(0));
}

#include <windowsx.h>

static LRESULT CALLBACK InputServiceWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (!s_pApp)
        return 0;

    POINT position;

    GetCursorPos(&position);
    ScreenToClient(GetActiveWindow(), &position);

    ProcessMouseMove(static_cast<uint16_t>(position.x), static_cast<uint16_t>(position.y));

    switch (uMsg)
    {
    case WM_CHAR:
        ProcessKeyboard(static_cast<uint16_t>(wParam), (lParam >> 16) & 0xFF, KEYEVENT_CHAR, false, false);
        break;
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDBLCLK: {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);
        int btnType = ((uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK)
                           ? 0
                           : ((uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK) ? 1 : 2));

        ProcessMouseButton(static_cast<uint16_t>(x), static_cast<uint16_t>(y), MBT_LEFT, true);
    }
    break;
    
    }

    return 0;
}
}

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

    InputServiceWndProc(aHwnd, aMsg, aWp, aLp);

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
   TP_HOOK_IAT2("User32.dll", "RegisterClassA", RegisterClassA_Hook);
   TP_HOOK_IAT2("User32.dll", "CreateWindowExA", CreateWindowExA_Hook);
   TP_HOOK_IAT2("User32.dll", "DefWindowProcA", DefWindowProcA_Hook);
});
