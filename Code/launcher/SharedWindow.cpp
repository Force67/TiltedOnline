
#include "SharedWindow.h"

SharedWindow::SharedWindow(WindowDelegate &aDelegate, glm::uvec2 aDefaultSize) : 
    m_delegate(aDelegate)
{
    Initialize(aDefaultSize);
}

SharedWindow::SharedWindow(WindowDelegate &aDelegate) : 
    m_delegate(aDelegate)
{
    glm::uvec2 defaultSz{
        GetSystemMetrics(SM_CXSCREEN), 
        GetSystemMetrics(SM_CYSCREEN)};
    Initialize(defaultSz);
}

SharedWindow::~SharedWindow()
{
    if (m_pHandle)
        CloseHandle(m_pHandle);
}

void SharedWindow::Initialize(glm::uvec2 aSize) noexcept
{
    WNDCLASSW wc{};
    wc.style = 3;
    wc.lpfnWndProc = StaticWndProc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.hIcon = LoadIconW(wc.hInstance, MAKEINTRESOURCEW(102));
    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(4));
    wc.lpszClassName = L"TPOnlineWindow";
    if (!RegisterClassW(&wc))
        return;

    // const DWORD windowStyle = 0x80000000; // 0xC00000
    const DWORD windowStyle = 0xC0000;
    const DWORD exStyle = 0;

    RECT rc{};
    SetRect(&rc, 0, 0, aSize.x, aSize.y);
    AdjustWindowRect(&rc, windowStyle, 0);

    glm::ivec2 adjustedSize{
        static_cast<int>(rc.right - rc.left), 
        static_cast<int>(rc.bottom - rc.top)};

    m_pHandle = CreateWindowExW(exStyle, wc.lpszClassName, L"TiltedOnline", windowStyle, CW_USEDEFAULT, 0,
                                adjustedSize.x, adjustedSize.y, nullptr, nullptr, wc.hInstance, this);
}

glm::uvec2 SharedWindow::GetWindowRect() const
{
    RECT rect;
    GetClientRect(m_pHandle, &rect);

    return {static_cast<int32_t>(rect.right - rect.left), static_cast<int32_t>(rect.bottom - rect.top)};
}

LRESULT SharedWindow::StaticWndProc(HWND aHwnd, UINT aMsg, WPARAM aWp, LPARAM aLp)
{
    auto* pWindow = reinterpret_cast<SharedWindow*>(GetWindowLongPtr(aHwnd, GWLP_USERDATA));
    if (aMsg == WM_NCCREATE)
    {
        if (!pWindow)
        {
            pWindow = reinterpret_cast<SharedWindow*>(reinterpret_cast<CREATESTRUCT*>(aLp)->lpCreateParams);
            SetWindowLongPtrW(aHwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
        }
    }

    if (pWindow)
    {
        if (pWindow->m_pChildProc)
            return pWindow->m_pChildProc(aHwnd, aMsg, aWp, aLp);

        pWindow->WndProc(aMsg, aWp, aLp);
    }

    return DefWindowProcW(aHwnd, aMsg, aWp, aLp);
}

void SharedWindow::WndProc(UINT aMsg, WPARAM aWp, LPARAM aLp)
{
    if (aMsg == WM_SIZE)
    {
        m_delegate.OnResize();
    }
}

bool SharedWindow::Poll() noexcept
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

    return !m_bShouldQuit;
}

void SharedWindow::Show(bool aToggle)
{
    if (m_pHandle)
    {
        ShowWindow(m_pHandle, aToggle ? SW_SHOW : SW_HIDE);
        if (aToggle)
        {
            SetForegroundWindow(m_pHandle);
            SetFocus(m_pHandle);
        }
    }
}

void SharedWindow::SetTitle(const wchar_t* aTitle)
{
    if (m_pHandle)
        SetWindowTextW(m_pHandle, aTitle); 
}

void SharedWindow::SetChildWndProc(WNDPROC aProc)
{
    m_pChildProc = aProc;
}

HWND SharedWindow::GetNativeHandle() const 
{
    return m_pHandle;
}
