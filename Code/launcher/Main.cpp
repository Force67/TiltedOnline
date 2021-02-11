
#include <TiltedCore/Stl.hpp>

#include "GameHost.h"
#include "TiltedOnlineApp.h"

struct ComInitializer
{
    ComInitializer()
    {
        CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    }
    ~ComInitializer()
    {
        CoUninitialize();
    }
};


std::wstring g_gamePath;

 LPWSTR WINAPI GetCommandLineW_Hook()
{
    static bool init_guard = false;
    if (!init_guard)
    {
 
        init_guard = true;
    }

    return GetCommandLineW();
}

void WINAPI RaiseException_Hook(DWORD dwExceptionCode, DWORD dwExceptionFlags, DWORD nNumberOfArguments,
                                const ULONG_PTR* lpArguments)
{
    if (dwExceptionCode == 0x406D1388 && !IsDebuggerPresent())
        return; // thread naming

    RaiseException(dwExceptionCode, dwExceptionFlags, nNumberOfArguments, lpArguments);
}

DWORD WINAPI GetModuleFileNameW_Hook(HMODULE hModule, LPWSTR lpFilename, DWORD nSize)
{
    if (!hModule)
    {
        wcscpy_s(lpFilename, nSize, g_gamePath.c_str());

        return (DWORD)g_gamePath.length();
    }

    return GetModuleFileNameW(hModule, lpFilename, nSize);
}

  static void InitEnvironment()
{
    const HMODULE hh = GetModuleHandleW(L"kernel32.dll");

    // in order to shut the compiler up
    if (!hh)
        return;

    static wchar_t game_dir[MAX_PATH] = LR"(D:\Games\steamapps\common\Skyrim Special Edition)";

    auto addDllDirectory = (decltype(&::AddDllDirectory))GetProcAddress(hh, "AddDllDirectory");
    auto setDefaultDllDirectories =
        (decltype(&::SetDefaultDllDirectories))GetProcAddress(hh, "SetDefaultDllDirectories");

    if (addDllDirectory && setDefaultDllDirectories)
    {
        setDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS | LOAD_LIBRARY_SEARCH_USER_DIRS);

        std::wstring appdir = MakeLocalAppPath();

        addDllDirectory(game_dir);
        addDllDirectory(appdir.c_str());
        SetCurrentDirectoryW(game_dir);
    }
    else
    {
        int32_t res = MessageBoxW(nullptr,
                                  L"Project77 requires Windows 7 patch KB2758857 to be installed in order to run.\n"
                                  L"Do you want to open the download page now ?",
                                  L"Project77", MB_YESNOCANCEL | MB_ICONSTOP);

        if (res == IDYES)
        {
            ShellExecuteW(nullptr, L"open", L"https://www.microsoft.com/en-us/download/details.aspx?id=35936", nullptr,
                         nullptr, SW_SHOWNORMAL);
        }
    }

    std::wstring pathBuf;
    pathBuf.resize(32768);

    GetEnvironmentVariableW(L"PATH", pathBuf.data(), pathBuf.length());

    // append bin & game directories
    std::wstring newPath = MakeLocalAppPath() + L";" + game_dir + L";" + pathBuf;
    SetEnvironmentVariableW(L"PATH", newPath.c_str());
}

int main(int argc, char** argv)
{
    ComInitializer comInit;

    const auto appInstance = TiltedPhoques::MakeUnique<TiltedOnlineApp>(argc, argv);
    if (!appInstance->Initialize())
    {
        return -1;
    }

    appInstance->Exec();

    #if 0
    InitEnvironment();

    static std::wstring gameDir = LR"(D:\Games\steamapps\common\Skyrim Special Edition\SkyrimSE.exe.unpacked.exe)";

    g_gamePath = gameDir;

    auto exeModule = GetModuleHandle(nullptr);
    p77_launcher::ExeHost loader(data);

    loader.LoadIntoModule(exeModule);

   #endif
    return 0;
}
