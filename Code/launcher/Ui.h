#pragma once

#if 0
class TiltedOnlineApp;

class UiClient final : public TiltedPhoques::OverlayClient
{
public:
    explicit UiClient(TiltedOnlineApp& aApp, TiltedPhoques::OverlayRenderHandler* apHandler);
    ~UiClient();

    bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process,
                                  CefRefPtr<CefProcessMessage> message) override;

  private:
    TiltedOnlineApp& m_app;
};
#endif
