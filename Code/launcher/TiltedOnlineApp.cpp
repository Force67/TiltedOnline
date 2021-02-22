
#include <cxxopts.hpp>
#include <Debug.hpp>

#include <TiltedCore/Filesystem.hpp>
#include <TiltedCore/Initializer.hpp>

#include "TiltedOnlineApp.h"
#include "loader/ExeLoader.h"
#include "Ui.h"

namespace fs = std::filesystem;

constexpr uintptr_t kGameLimit = 0x140000000 + 0x65000000;

extern bool BootstrapGame(TiltedOnlineApp*);
extern void WINDOWHACK_INIT(TiltedPhoques::OverlayApp* pApp);

TiltedOnlineApp::TiltedOnlineApp(int argc, char** argv)
{
    using TiltedPhoques::Debug;
    // only creates a new console if we aren't started from one
    Debug::CreateConsole();

    cxxopts::Options options(argv[0], "Welcome to the TiltedOnline command line (^_^)/");

    std::string gameName = "";
    options.add_options()
        ("g,game", "game name", cxxopts::value<std::string>(gameName))
        ("r,reselect", "Reselect the game path");
    try
    {
        const auto result = options.parse(argc, argv);
        if (!gameName.empty())
        {
            if ((m_titleId = ToTitleId(gameName)) == TitleId::kUnknown)
            {
                fmt::print("Unable to determine game type");
                m_appState = AppState::kFailed;
                return;
            }

            // signal that we don't want the ui
            m_appState = AppState::kInGame;
        }

        m_bReselectFlag = result.count("reselect");
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

    if (m_pGameClientHandle)
        FreeLibrary(m_pGameClientHandle);
}

std::filesystem::path& TiltedOnlineApp::GetGamePath()
{
    return m_gamePath;
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

    auto& geom = m_pWindow->GetWindowDimensions();
    auto res = m_pRenderer->Create(m_pWindow->GetNativeHandle(), geom.x, geom.y);
    if (res != RendererD3d11::Result::kSuccess)
    {
        // TODO: log result
        return false;
    }

    // jump directly in game
    if (m_appState == AppState::kInGame)
    {
        m_pWindow->Show();
        StartGame(m_titleId);
        return true;
    }

    // everything looks alright, lets launch into space
    m_pCefApp = MakeUnique<OverlayApp>(m_pRenderer->CreateRenderProvider(), L"TPProcess.exe");
    m_pCefApp->Initialize(new UiClient(*this, m_pCefApp->HACK_GetRenderProvider()->Create()));

    m_pClient = m_pCefApp->GetClient();

    WINDOWHACK_INIT(m_pCefApp.get());
    m_pWindow->Show();
    

    return true;
}

void TiltedOnlineApp::StartGame(TitleId aTid)
{
    // if the title id isn't unknown launch params take precedence, but
    // this shouldn't happen
    if (m_titleId == TitleId::kUnknown)
        m_titleId = aTid;

    ExeLoader::entrypoint_t start = nullptr;
    {
        fs::path exeName;
        bool result = FindTitlePath(m_titleId, m_bReselectFlag, m_gamePath, exeName);

        auto fullPath = m_gamePath / exeName;
        if (!result || !fs::exists(fullPath))
        {
            MessageBoxW(nullptr, L"Failed to find a game path", L"TiltedPhoques", MB_OK);
            return;
        }

        if (!BootstrapGame(this))
        {
            return;
        }

        ExeLoader loader(kGameLimit, GetProcAddress);
        if (!loader.Load(fullPath))
            return;

        start = loader.GetEntryPoint();
    }

    // apply all game hooks
    Initializer::RunAll();

    // this starts the game
    start();

    // game returned???!?
}

void TiltedOnlineApp::LoadClient()
{
    WString clientName = ToClientName(m_titleId);

    auto clientPath = TiltedPhoques::GetPath() / clientName;
    m_pGameClientHandle = LoadLibraryW(clientPath.c_str());

    if (!m_pGameClientHandle)
    {
        auto errMsg = fmt::format(L"Unable to load {}", clientPath.native());

        MessageBoxW(nullptr, errMsg.c_str(), L"TiltedOnline", MB_OK);

        // todo: think about if we can kill the game

    }
}

void TiltedOnlineApp::HandleMessage(Window::EventType type)
{
    if (type == Window::EventType::kResized)
    {
        if (m_pRenderer->GetSwapChain())
        {
            auto& geom = m_pWindow->GetWindowDimensions();
            m_pRenderer->Resize(geom.x, geom.y);
        }
    }
}

int32_t TiltedOnlineApp::Exec()
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

    return 0;
}
