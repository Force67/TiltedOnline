
#include <spdlog/spdlog.h>

#include "TitleSelect.h"
#include "utils/Registry.h"

constexpr wchar_t kRegistryPath[] = L"Software\\TiltedPhoques\\TiltedOnline\\";

WString ToClientName(TitleId id) noexcept
{
    switch (id)
    {
    case TitleId::kSkyrimSE:
        return L"STClient.dll";
    case TitleId::kSyrimVR:
        return L"STVRClient.dll";
    case TitleId::kFallout4:
        return L"FTClient.dll";
    case TitleId::kFallout4VR:
        return L"FTVRClient.dll";
    default:
        return L"";
    }
}

WString ToGameName(TitleId id) noexcept
{
    switch (id)
    {
    case TitleId::kSkyrimSE:
        return L"Skyrim Special Edition";
    case TitleId::kSyrimVR:
        return L"Skyrim VR";
    case TitleId::kFallout4:
        return L"Fallout4";
    case TitleId::kFallout4VR:
        return L"Fallout4 VR";
    default:
        return L"";
    }
}

TitleId ToTitleId(std::string_view aName) noexcept
{
    if (aName == "SkyrimSE")
        return TitleId::kSkyrimSE;

    if (aName == "Fallout4")
        return TitleId::kFallout4;

    if (aName == "SkyrimVR")
        return TitleId::kSyrimVR;

    return TitleId::kUnknown;
}

static std::wstring TryFindDefaultPath(TitleId aTitleId)
{
    auto path = WString(L"Software\\Wow6432Node\\Bethesda Softworks\\") + ToGameName(aTitleId);

    // thanks bethesda
    const wchar_t* subName = 
        aTitleId == TitleId::kFallout4 || 
        aTitleId == TitleId::kFallout4VR ? L"Installed Path" : L"installed path";

    return Registry::ReadString<wchar_t>(HKEY_LOCAL_MACHINE, path.c_str(), subName);
}

bool FindTitlePath(TitleId aTitle, bool aForceReselect, fs::path& aPathOut, fs::path& aExeOut)
{
    auto path = WString(kRegistryPath) + ToGameName(aTitle);

    // separate, so you can use your own executable if you want :D
    aPathOut = Registry::ReadString<wchar_t>(HKEY_CURRENT_USER, path.c_str(), L"GamePath");
    aExeOut = Registry::ReadString<wchar_t>(HKEY_CURRENT_USER, path.c_str(), L"GameExe");

    if (aPathOut.empty() || aExeOut.empty() || aForceReselect)
    {
        OPENFILENAMEW file{};

        std::wstring buffer;
        buffer.resize(MAX_PATH);

        file.lpstrFile = buffer.data();
        file.lStructSize = sizeof(file);
        file.nMaxFile = MAX_PATH;
        file.lpstrFilter = L"Executables\0*.exe\0";
        file.lpstrDefExt = L"EXE";
        file.lpstrTitle = L"Please select your Game executable (*.exe)";
        file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER;

        auto initialDir = TryFindDefaultPath(aTitle);
        file.lpstrInitialDir = initialDir.empty() ? buffer.data() : initialDir.data();

        if (!GetOpenFileNameW(&file))
        {
            MessageBoxW(nullptr, L"Failed to retrieve gamepath. Cannot launch the game!", L"TiltedPhoques",
                        MB_ICONSTOP);
            return false;
        }

        size_t pos = buffer.find_last_of(L'\\');
        if (pos == std::string::npos)
            return false;

        aExeOut = &buffer[pos + 1];
        aPathOut = buffer.substr(0, pos);

        return Registry::WriteString(HKEY_CURRENT_USER, path.c_str(), L"GamePath", aPathOut.native()) &&
               Registry::WriteString(HKEY_CURRENT_USER, path.c_str(), L"GameExe", aExeOut.native());
    }

    return true;
}
