#pragma once

#include <spdlog/spdlog.h>
#include <TiltedCore/Stl.hpp>
#include <TiltedCore/Filesystem.hpp>

#include "Window.h"
#include "RendererD3d11.h"

using namespace TiltedPhoques;

class TiltedOnlineApp final : public Window::MessageHandler
{
 public:
    enum class GameId
    {
        kSkyrimSE = 2 << 0,
        kFallout4 = 2 << 1,
        kSkyrimVRFlavoured = 2 << 2,
        kGameUnknown,
    };

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

    void Exec();

    static GameId ToGameId(std::string_view name);

  private:
    void StartGame();
    bool FindGamePath();

    void HandleMessage(Window::EventType aType) override;

    AppState m_appState{AppState::kStarting};
    GameId m_gameId{GameId::kGameUnknown};

    UniquePtr<Window> m_pWindow;
    UniquePtr<RendererD3d11> m_pRenderer;
    OverlayClient* m_pClient = nullptr;
    UniquePtr<OverlayApp> m_pCefApp;

    bool m_bReselectFlag = false;
    std::filesystem::path m_gamePath;
};
