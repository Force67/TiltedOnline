#pragma once

#include <Windows.h>
#include <TiltedCore/Stl.hpp>

using namespace TiltedPhoques;

class Window
{
public:
    enum class EventType
    {
        kNone,
        kClosed,
        kResized,
        kLostFocus,
        GainedFocus,
    };

    class MessageHandler
    {
      public:
        virtual ~MessageHandler() = default;
        virtual void HandleMessage(EventType) = 0;
    };

    struct WindowDimensions
    {
        int32_t x, y;
    };

    Window(MessageHandler&);
    ~Window();

    bool Create(const wchar_t* aWindowName, int32_t aWidth = 0, int32_t aHeight = 0);

    void Show();

    // polling states
    bool ShouldClose() const;

    // update the window event loop
    void PollEvents();

    static void GetOptimalResolution(int32_t& x, int32_t& y);

    const HWND GetNativeHandle() const;
    WindowDimensions GetWindowDimensions() const;

  private:
    static LRESULT CALLBACK StaticWndProc(HWND aHwnd, UINT aMsg, WPARAM aWp, LPARAM aLp);

    void HandleMessage(UINT aMsg, WPARAM aWp, LPARAM aLp);

    HWND m_pHwnd = nullptr;
    bool m_bShouldQuit = false;
    MessageHandler& m_Handler;
};
