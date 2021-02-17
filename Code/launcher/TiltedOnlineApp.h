#pragma once

#include <spdlog/spdlog.h>

#include <TiltedCore/Stl.hpp>
#include <TiltedCore/Filesystem.hpp>
#include <TiltedCore/Platform.hpp>

#include "Window.h"
#include "TitleSelect.h"
#include "RendererD3d11.h"

class TiltedOnlineApp final : public Window::MessageHandler
{
 public:
    enum class AppState
    {
        kFailed,
        kStarting,
        kInGame,
        kBackground
    };

    TiltedOnlineApp(int argc, char** argv);
    ~TiltedOnlineApp();

    bool Initialize();
    void LoadClient();

    int32_t Exec();

    std::filesystem::path& GetGamePath();
  private:
    void StartGame();
    void HandleMessage(Window::EventType aType) override;

    AppState m_appState{AppState::kStarting};
    TitleId m_titleId{TitleId::kUnknown};

    UniquePtr<Window> m_pWindow;
    UniquePtr<RendererD3d11> m_pRenderer;
    OverlayClient* m_pClient = nullptr;
    UniquePtr<OverlayApp> m_pCefApp;

    bool m_bReselectFlag = false;
    HMODULE m_pGameClientHandle = nullptr;
    fs::path m_gamePath;
};
