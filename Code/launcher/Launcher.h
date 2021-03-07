#pragma once

#include <spdlog/spdlog.h>

#include <TiltedCore/Stl.hpp>
#include <TiltedCore/Filesystem.hpp>
#include <TiltedCore/Platform.hpp>
#include "TitleSelect.h"

#include "SharedWindow.h"

class SharedPipeline;
class LauncherUI;

class Launcher final : public WindowDelegate
{
 public:
    explicit Launcher(int argc, char** argv);
    ~Launcher();

    bool Initialize();
    void LoadClient() noexcept;
    void RunTitle(TitleId aTid);

    int32_t Exec() noexcept;

    const fs::path& GetGamePath() const;
    const fs::path& GetExePath() const;

    inline SharedWindow& GetWindow() const { return *m_pWindow; }
    inline SharedPipeline& GetPipeline() const { return *m_pPipeline; }

private:
    void ParseCommandline(int aArgc, char** aArgv);
    void OnResize();

private:
    enum class AppState
    {
        kFailed,
        kStarting,
        kInGame,
        kBackground
    } m_appState{AppState::kStarting};
    TitleId m_titleId{TitleId::kUnknown};
    fs::path m_gamePath;
    fs::path m_exePath;
    bool m_bReselectFlag = false;

    HMODULE m_pGameClientHandle = nullptr;
    UniquePtr<LauncherUI> m_pUI;
    UniquePtr<SharedWindow> m_pWindow;
    UniquePtr<SharedPipeline> m_pPipeline;
};

Launcher* GetLauncher();
