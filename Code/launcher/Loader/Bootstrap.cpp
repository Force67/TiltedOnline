
#include <Windows.h>
#include <TiltedOnlineApp.h>
#include <FunctionHook.hpp>
#include <TiltedCore/Initializer.hpp>

static TiltedOnlineApp* g_pAppInstance = nullptr;

void GetStartupInfoW_Hook(LPSTARTUPINFOW apInfo) noexcept
{
    static bool once = false;
    if (!once)
    {
        g_pAppInstance->LoadClient();
        once = true;
    }

    GetStartupInfoW(apInfo);
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
        auto& path = g_pAppInstance->GetGamePath();
        wcscpy_s(lpFilename, nSize, path.c_str());

        return (DWORD)path.native().length();
    }

    return GetModuleFileNameW(hModule, lpFilename, nSize);
}

bool BootstrapGame(TiltedOnlineApp* apAppInstance)
{
    g_pAppInstance = apAppInstance;

    static auto s_AddDllDirectory =
        (decltype(&::AddDllDirectory))GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "AddDllDirectory");
    static auto s_SetDefaultDllDirectory = (decltype(&::SetDefaultDllDirectories))GetProcAddress(
        GetModuleHandleW(L"kernel32.dll"), "SetDefaultDllDirectories");

    if (!s_AddDllDirectory || !s_SetDefaultDllDirectory)
    {
        int32_t res = MessageBoxW(
            nullptr,
            L"TiltedOnline requires Windows 7 patch KB2758857 to be installed in order to run.\n"
            L"Do you want to open the download page now ?",
            L"TiltedOnline", MB_YESNOCANCEL | MB_ICONSTOP);

        if (res == IDYES)
        {
            ShellExecuteW(nullptr, L"open", L"https://www.microsoft.com/en-us/download/details.aspx?id=35936", nullptr,
                          nullptr, SW_SHOWNORMAL);
        }
    
        return false;
    }

    auto appPath = TiltedPhoques::GetPath();
    auto gamePath = apAppInstance->GetGamePath();

    s_SetDefaultDllDirectory(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS | LOAD_LIBRARY_SEARCH_USER_DIRS);
    s_AddDllDirectory(appPath.c_str());
    s_AddDllDirectory(gamePath.c_str());
    SetCurrentDirectoryW(gamePath.c_str());

    std::wstring pathBuf;
    pathBuf.resize(32768);
    GetEnvironmentVariableW(L"PATH", pathBuf.data(), pathBuf.length());

    // append bin & game directories
    std::wstring newPath = appPath.wstring() + L";" + gamePath.wstring() + L";" + pathBuf;
    SetEnvironmentVariableW(L"PATH", newPath.c_str());
    return true;
}

static TiltedPhoques::Initializer s_Init([] {
    TiltedPhoques::HookIATIm(nullptr, "Kernel32.dll", "GetStartupInfoW", GetStartupInfoW_Hook);
    TiltedPhoques::HookIATIm(nullptr, "Kernel32.dll", "GetModuleFileNameW", GetModuleFileNameW_Hook);
    TiltedPhoques::HookIATIm(nullptr, "Kernel32.dll", "RaiseException", RaiseException_Hook);
});
