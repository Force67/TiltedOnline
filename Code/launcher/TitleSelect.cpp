
#include <Windows.h>
#include <spdlog/spdlog.h>
#include "TitleSelect.h"

#include "utils/Registry.h"
#include "utils/Error.h"

constexpr wchar_t kRegistryPath[] = LR"(Software\TiltedPhoques\TiltedOnline\)";

WString ToClientName(TitleId aTid) noexcept
{
    switch (aTid)
    {
    case TitleId::kSkyrimSE:
        return L"SkyrimTogether.dll";
    case TitleId::kSyrimVR:
        return L"SkyrimTogetherVR.dll";
    case TitleId::kFallout4:
        return L"FalloutTogether.dll";
    case TitleId::kFallout4VR:
        return L"FalloutTogetherVR.dll";
    default:
        return L"";
    }
}

WString ToGameName(TitleId aTid) noexcept
{
    switch (aTid)
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

uint32_t ToSteamAppId(TitleId aTid) noexcept
{
    switch (aTid)
    {
    case TitleId::kSkyrimSE:
        return 489830;
    case TitleId::kSyrimVR:
        return 611670;
    case TitleId::kFallout4:
        return 377160;
    case TitleId::kFallout4VR:
        return 611660;
    default:
        return 0;
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

static std::wstring SuggestTitlePath(TitleId aTitleId)
{
    auto path = WString(LR"(Software\Wow6432Node\Bethesda Softworks\)") + ToGameName(aTitleId);

    const wchar_t* subName;
    switch (aTitleId)
    {
    case TitleId::kSkyrimSE:
    case TitleId::kSyrimVR:
        subName = L"installed path";
        break;
    case TitleId::kFallout4: 
    case TitleId::kFallout4VR:
        subName = L"Installed Path";
        break;
    case TitleId::kReserved1:
    case TitleId::kReserved2:
    default:
        subName = L"";
    }

    return Registry::ReadString<wchar_t>(HKEY_LOCAL_MACHINE, path.c_str(), subName);
}

bool FindTitlePath(TitleId aTid, bool aForceReselect, fs::path& aTitlePath, fs::path& aExePath)
{
    const WString regPath = WString(kRegistryPath) + ToGameName(aTid);

    // separate, so a custom executable can be chosen for TP
    aTitlePath = Registry::ReadString<wchar_t>(HKEY_CURRENT_USER, regPath.c_str(), L"TitlePath");
    aExePath = Registry::ReadString<wchar_t>(HKEY_CURRENT_USER, regPath.c_str(), L"TitleExe");

    if (!fs::exists(aTitlePath) || !fs::exists(aExePath) || aForceReselect)
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

        auto initialDir = SuggestTitlePath(aTid);
        file.lpstrInitialDir = initialDir.empty() ? buffer.data() : initialDir.data();

        if (!GetOpenFileNameW(&file))
        {
            FatalError(L"Failed to select path. Cannot launch!\nTry reselecting the path by launching TiltedOnline "
                       L"with the '-r' parameter.");
            return false;
        }

        size_t pos = buffer.find_last_of(L'\\');
        if (pos == std::string::npos)
            return false;

        aTitlePath = buffer.substr(0, pos);
        aExePath = buffer;

        return Registry::WriteString(HKEY_CURRENT_USER, regPath.c_str(), L"TitlePath", aTitlePath.native()) &&
               Registry::WriteString(HKEY_CURRENT_USER, regPath.c_str(), L"TitleExe", buffer);
    }

    return true;
}
