#pragma once

#include <Windows.h>
#include <glm/glm.hpp>

class WindowDelegate
{
public:
    virtual ~WindowDelegate() = default;

    virtual void OnResize() = 0;
};

class SharedWindow final
{
public:
    class MessageHandler
    {
    public:
        virtual ~MessageHandler() = default;
        virtual void HandleMessage(UINT aMsg, WPARAM aWp, LPARAM aLp) = 0;
    };

    explicit SharedWindow(WindowDelegate &aDelegate, glm::uvec2 aDefaultSize);
    explicit SharedWindow(WindowDelegate &aDelegate);
    ~SharedWindow();

    void Initialize(glm::uvec2 aSize) noexcept;
    bool Poll() noexcept;

    glm::uvec2 GetWindowRect() const;
    HWND GetNativeHandle() const;

    void Show(bool aToggle = true);
    void SetTitle(const wchar_t* aTitle);
    void SetChildWndProc(WNDPROC aProc);

private:
    void WndProc(UINT aMsg, WPARAM aWp, LPARAM aLp);
    static LRESULT CALLBACK StaticWndProc(HWND aHwnd, UINT aMsg, WPARAM aWp, LPARAM aLp);

private:

    bool m_bShouldQuit = false;
    HWND m_pHandle = nullptr;
    WNDPROC m_pChildProc = nullptr;
    WindowDelegate& m_delegate;
};
