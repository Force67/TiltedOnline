
#include "Ui.h"
#include "TiltedOnlineApp.h"
#include "TitleSelect.h"

UiClient::UiClient(TiltedOnlineApp& aApp, TiltedPhoques::OverlayRenderHandler* apHandler) : 
    TiltedPhoques::OverlayClient(apHandler), m_app(aApp)
{
    
}

UiClient::~UiClient() 
{
}

bool UiClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
    if (message->GetName() == "ui-event")
    {
        __debugbreak();
        return true;
    }


    if (message->GetName() == "startGame")
    {
        auto pArgs = message->GetArgumentList();
        auto tid = static_cast<TitleId>(pArgs->GetInt(0)); 
    
        m_app.StartGame(tid);
        return true;
    }

    return false;
}
