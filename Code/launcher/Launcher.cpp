
#include <cxxopts.hpp>
#include <Debug.hpp>

#include <TiltedCore/Filesystem.hpp>
#include <TiltedCore/Initializer.hpp>

#include "Launcher.h"
#include "SteamSupport.h"

#include "loader/ExeLoader.h"
#include "common/BuildInfo.h"
#include "Utils/Error.h"

#include "LauncherUI.h"
#include "SharedWindow.h"
#include "SharedPipeline.h"

Launcher* g_pLauncher = nullptr;
constexpr uintptr_t kGameLoadLimit = 0x140000000 + 0x70000000;

extern void BootstrapGame(Launcher* apLauncher);

Launcher* GetLauncher() 
{
    return g_pLauncher;
}

Launcher::Launcher(int argc, char** argv)
{
    using TiltedPhoques::Debug;

    Debug::CreateConsole();
    ParseCommandline(argc, argv);

    m_pWindow = MakeUnique<SharedWindow>(*this);
    m_pPipeline = MakeUnique<SharedPipeline>();
    m_pUI = MakeUnique<LauncherUI>(*m_pWindow, *m_pPipeline);

    g_pLauncher = this;
}

Launcher::~Launcher()
{
    // explicit
    if (m_pGameClientHandle)
        FreeLibrary(m_pGameClientHandle);

    g_pLauncher = nullptr;
}

void Launcher::ParseCommandline(int aArgc, char** aArgv)
{
    cxxopts::Options options(aArgv[0], 
        R"(Welcome to the TiltedOnline command line \(^_^)/)");

    std::string gameName = "";
    options.add_options()
        ("h,help", "Display the help message")
        ("v,version", "Display the build version")
        ("g,game", "game name (SkyrimSE or Fallout4)", 
            cxxopts::value<std::string>(gameName))
        ("r,reselect", "Reselect the game path");
    try
    {
        const auto result = options.parse(aArgc, aArgv);

        if (result.count("help"))
        {
            fmt::print(options.help({""}));
            m_appState = AppState::kFailed;
            return;
        }

        if (result.count("version"))
            fmt::print("TiltedOnline version: " BUILD_BRANCH "@" 
                BUILD_COMMIT " built on " BUILD_TIMESTAMP "\n");

        if (!gameName.empty())
        {
            if ((m_titleId = ToTitleId(gameName)) == TitleId::kUnknown)
            {
                fmt::print("Unable to determine game type\n");
                m_appState = AppState::kFailed;
                return;
            }

            // signal that we don't want the ui
            m_appState = AppState::kInGame;
        }

        m_bReselectFlag = result.count("reselect");
    }
    catch (const cxxopts::OptionException& ex)
    {
        m_appState = AppState::kFailed;
        fmt::print("Exception while parsing options: {}\n", ex.what());
    }
}

const fs::path& Launcher::GetGamePath() const
{
    return m_gamePath;
}

const fs::path& Launcher::GetExePath() const
{
    return m_exePath;
}

bool Launcher::Initialize()
{
    // there has been an error during startup
    if (m_appState == AppState::kFailed)
    {
        return false;
    }

    if (!m_pWindow->GetNativeHandle())
    {
        FatalError(L"Failed to create window!");
        return false;
    }

    const auto result = m_pPipeline->Create(*m_pWindow);
    if (result != SharedPipeline::Result::kSuccess)
    {
        auto errMsg = fmt::format(L"Failed to create pipeline\nError code: {}", 
            static_cast<int>(result));
    
        FatalError(errMsg.c_str());
        return false;
    }

    m_pWindow->Show();

    // no further initialization needed
    if (m_appState == AppState::kInGame)
    {
        return true;
    }

    return true;
}

void Launcher::RunTitle(TitleId aTid)
{
    // if the title id isn't unknown launch params take precedence, but
    // this shouldn't happen
    if (m_titleId == TitleId::kUnknown)
        m_titleId = aTid;

    ExeLoader::TEntryPoint start = nullptr;
    {
        if (!FindTitlePath(m_titleId, m_bReselectFlag, m_gamePath, m_exePath))
            return;

        BootstrapGame(this);
        SteamLoad(m_titleId, m_gamePath);

        ExeLoader loader(kGameLoadLimit, GetProcAddress);
        if (!loader.Load(m_exePath))
            return;

        start = loader.GetEntryPoint();
    }

    Initializer::RunAll();
    start();


    __debugbreak();
}

void Launcher::LoadClient() noexcept
{
    WString clientName = ToClientName(m_titleId);

    auto clientPath = TiltedPhoques::GetPath() / clientName;
    m_pGameClientHandle = LoadLibraryW(clientPath.c_str());

    if (!m_pGameClientHandle)
    {
        auto fmt = fmt::format(L"Failed to load client\nPath: {}", clientPath.native());

        FatalError(fmt.c_str());
        TerminateProcess(GetCurrentProcess(), 0);
    }
}

void Launcher::OnResize()
{
    if (m_pPipeline->GetSwapChain())
    {
        const auto geom = m_pWindow->GetWindowRect();
        m_pPipeline->Resize(geom);
    }
}

int32_t Launcher::Exec() noexcept
{
    if (m_appState == AppState::kInGame)
    {
        RunTitle(m_titleId);
        return 0;
    }

    while (m_pWindow->Poll())
    {
        m_pPipeline->BeginDraw();
        m_pUI->Draw();
        m_pPipeline->EndDraw();
    }

    return 0;
}
