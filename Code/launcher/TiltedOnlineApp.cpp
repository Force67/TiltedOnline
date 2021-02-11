
#include <cxxopts.hpp>
#include <Debug.hpp>
#include <Registry.h>

#include <TiltedCore/Filesystem.hpp>
#include <TiltedCore/Initializer.hpp>

#include "GameLoader.h"
#include "TiltedOnlineApp.h"

namespace fs = std::filesystem;

constexpr uintptr_t kGameLimit = 0x140000000 + 0x65000000;

TiltedOnlineApp::GameId TiltedOnlineApp::ToGameId(std::string_view aName)
{
    if (aName == "SkyrimSE")
        return GameId::kSkyrimSE;

    if (aName == "Fallout4")
        return GameId::kFallout4;

    if (aName == "SkyrimVR")
        return GameId::kSkyrimVRFlavoured;

    return GameId::kGameUnknown;
}

TiltedOnlineApp::TiltedOnlineApp(int argc, char** argv)
{
    using TiltedPhoques::Debug;
    // only creates a new console if we aren't started from one
    Debug::CreateConsole();

    cxxopts::Options options(argv[0], "Welcome to the TiltedOnline command line (^_^)/");

    std::string gameName = "";
    options.add_options()
        ("g,game", "game name", cxxopts::value<std::string>(gameName),
        ("r,reselect", "Reselect the game path"));
    try
    {
        const auto result = options.parse(argc, argv);
        if (!gameName.empty())
        {
            if ((m_gameId = ToGameId(gameName)) == GameId::kGameUnknown)
            {
                fmt::print("Unable to determine game type");
                m_appState = AppState::kFailed;
                return;
            }

            // signal that we don't want the ui
            m_appState = AppState::kInGame;
        }

        if (result.count("reselect"))
            m_bReselectFlag = true;

        m_pWindow = MakeUnique<Window>(*this);
        m_pRenderer = MakeUnique<RendererD3d11>();
    }
    catch (const cxxopts::OptionException& ex)
    {
        m_appState = AppState::kFailed;
        fmt::print("Exception while parsing options: {}\n", ex.what());
    }
}

TiltedOnlineApp::~TiltedOnlineApp()
{
    m_pClient->GetBrowser()->GetHost()->CloseBrowser(true);

    if (m_pCefApp)
        m_pCefApp->Shutdown();
}

bool TiltedOnlineApp::Initialize()
{
    // there has been an error during startup
    if (m_appState == AppState::kFailed)
    {
        return false;
    }

    if (!m_pWindow->Create(L"TiltedOnline"))
    {
        return false;
    }

    // jump directly in game
    if (m_appState == AppState::kInGame)
    {
        StartGame();
        return true;
    }

    auto& geom = m_pWindow->GetWindowDimensions();
    RendererD3d11::Result res = m_pRenderer->Create(
        m_pWindow->GetNativeHandle(), geom.x, geom.y);

    if (res != RendererD3d11::Result::kSuccess)
    {
        return false;
    }

    // everything looks alright, lets launch into space
    m_pCefApp = MakeUnique<OverlayApp>(m_pRenderer->CreateRenderProvider(), L"TPProcess.exe");
    m_pCefApp->Initialize();

    m_pClient = m_pCefApp->GetClient();
    m_pWindow->Show();

    return true;
}

bool TiltedOnlineApp::FindGamePath()
{
    const std::wstring pathName = fmt::format(L"GamePath_{}", m_gameId);

    m_gamePath = Registry::Read<std::wstring>(
        Registry::Key::CurrentUser, L"SOFTWARE", L"TiltedPhoques", pathName);

    if (m_gamePath.empty() || m_bReselectFlag)
    {
        OPENFILENAMEW file{};

        wchar_t buf[MAX_PATH]{};
        file.lStructSize = sizeof(file);
        file.nMaxFile = MAX_PATH;
        file.lpstrFilter = L"Executables\0*.exe\0";
        file.lpstrDefExt = L"EXE";
        file.lpstrTitle = L"Please select your Game executable (*.exe)";
        file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER;

        file.lpstrFile = buf;
        file.lpstrInitialDir = buf;

        if (!GetOpenFileNameW(&file))
        {
            MessageBoxW(nullptr, L"Failed to retrive gamepath. Cannot launch the game!", L"TiltedPhoques", MB_ICONSTOP);
            return false;
        }

        for (size_t i = wcslen(buf); i > 0; --i)
        {
            if (buf[i] == '\\')
            {
                buf[i + 1] = 0;
                break;
            }
        }

        m_gamePath = std::wstring(buf);

        if (!fs::exists(m_gamePath))
        {
            // TODO: invalid!!!!!
        }

        // TODO: write registry

        //Registry::Write();
    
    }
}

void TiltedOnlineApp::StartGame()
{
    // apply all game hooks
    Initializer::RunAll();

    GameLoader loader(kGameLimit);
    loader.Load(m_gamePath);

    return loader.Execute();
}

void TiltedOnlineApp::HandleMessage(Window::EventType type)
{
    if (type == Window::EventType::kResized)
    {
        auto& geom = m_pWindow->GetWindowDimensions();
        m_pRenderer->Resize(geom.x, geom.y);
    }
}

void TiltedOnlineApp::Exec()
{
    m_pClient->Create();

    while (!m_pWindow->ShouldClose())
    {
        m_pRenderer->Begin();
        m_pClient->Render();
        m_pRenderer->End();

        // ask for new events
        m_pWindow->PollEvents();
    }
}
