/*
 * This file is part of the CitizenFX project - http://citizen.re/
 *
 * See LICENSE and MENTIONS in the root of the source tree for information
 * regarding licensing.
 */

#include <algorithm>
#include "GameLoader.h"

#include <TiltedCore/Filesystem.hpp>

GameLoader::GameLoader(uintptr_t aLoadLimit) : 
    m_loadLimit(aLoadLimit)
{}

void GameLoader::LoadImports(const IMAGE_NT_HEADERS* apNtHeader)
{
    const auto* importDirectory = &apNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    const auto* descriptor = GetTargetRVA<const IMAGE_IMPORT_DESCRIPTOR>(importDirectory->VirtualAddress);

    while (descriptor->Name)
    {
        const char* name = GetTargetRVA<char>(descriptor->Name);

        HMODULE module = LoadLibraryA(name);
        if (!module)
        {
            continue;
        }

        // "don't load"
        if (reinterpret_cast<uint32_t>(module) == 0xFFFFFFFF)
        {
            descriptor++;
            continue;
        }

        auto nameTableEntry = GetTargetRVA<uintptr_t>(descriptor->OriginalFirstThunk);
        auto addressTableEntry = GetTargetRVA<uintptr_t>(descriptor->FirstThunk);

        if (!descriptor->OriginalFirstThunk)
        {

            nameTableEntry = GetTargetRVA<uintptr_t>(descriptor->FirstThunk);
        }

        while (*nameTableEntry)
        {
            FARPROC function;
            const char* functionName;

            // is this an ordinal-only import?
            if (IMAGE_SNAP_BY_ORDINAL(*nameTableEntry))
            {
                function = GetProcAddress(module, MAKEINTRESOURCEA(IMAGE_ORDINAL(*nameTableEntry)));
            }
            else
            {
                auto import = GetTargetRVA<IMAGE_IMPORT_BY_NAME>(*nameTableEntry);

                function = (FARPROC)GetProcAddress(module, import->Name);
                functionName = import->Name;
            }

            if (!function)
            {
                char pathName[MAX_PATH];
                GetModuleFileNameA(module, pathName, sizeof(pathName));
            }

            *addressTableEntry = (uintptr_t)function;

            nameTableEntry++;
            addressTableEntry++;
        }

        descriptor++;
    }
}

void GameLoader::LoadSections(const IMAGE_NT_HEADERS* apNtHeader)
{
    auto* section = IMAGE_FIRST_SECTION(apNtHeader);
    for (int i = 0; i < apNtHeader->FileHeader.NumberOfSections; i++)
    {
        void* targetAddress = GetTargetRVA<uint8_t>(section->VirtualAddress);
        const void* sourceAddress = m_pBinary + section->PointerToRawData;

        if ((uintptr_t)targetAddress >= m_loadLimit)
        {
            return;
        }

        if (section->SizeOfRawData > 0)
        {
            uint32_t sizeOfData = std::min(section->SizeOfRawData, section->Misc.VirtualSize);

            memcpy(targetAddress, sourceAddress, sizeOfData);

            DWORD oldProtect;
            VirtualProtect(targetAddress, sizeOfData, PAGE_EXECUTE_READWRITE, &oldProtect);
        }

        section++;
    }
}

void GameLoader::Load(std::filesystem::path &source)
{
    FILE* file = _wfopen(source.c_str(), L"rb");
    if (!file)
    {
        return;
    }

    // load the game into a buffer
    fseek(file, 0, SEEK_END);
    auto length = ftell(file);

    auto data = std::make_unique<uint8_t[]>(length);

    fseek(file, 0, SEEK_SET);
    fread(data.get(), 1, length, file);
    fclose(file);

    m_moduleHandle = GetModuleHandleW(nullptr);

    // the target module
    const auto* dosHeader = GetRVA<const IMAGE_DOS_HEADER>(0);
    if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
    {
        return;
    }

    const auto* ntHeader = GetRVA<const IMAGE_NT_HEADERS>(dosHeader->e_lfanew);

    auto* sourceHeader = GetTargetRVA<IMAGE_DOS_HEADER>(0);
    auto* sourceNtHeader = GetTargetRVA<IMAGE_NT_HEADERS>(sourceHeader->e_lfanew);

    m_pEntryPoint = GetTargetRVA<void>(ntHeader->OptionalHeader.AddressOfEntryPoint);

    LoadSections(ntHeader);
    LoadImports(ntHeader);

    // copy over TLS index (source in this case indicates the TLS data to copy from, which is the launcher app itself)
    if (ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size)
    {
        const auto* targetTls = GetTargetRVA<IMAGE_TLS_DIRECTORY>(
            sourceNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
        const auto* sourceTls = GetTargetRVA<IMAGE_TLS_DIRECTORY>(
            ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);

        *(DWORD*)(sourceTls->AddressOfIndex) = 0;

        LPVOID tlsBase = *(LPVOID*)__readgsqword(0x58);

        DWORD oldProtect;
        VirtualProtect(reinterpret_cast<LPVOID>(targetTls->StartAddressOfRawData),
                       sourceTls->EndAddressOfRawData - sourceTls->StartAddressOfRawData, PAGE_READWRITE, &oldProtect);

        memcpy(tlsBase, reinterpret_cast<void*>(sourceTls->StartAddressOfRawData),
               sourceTls->EndAddressOfRawData - sourceTls->StartAddressOfRawData);
        memcpy((void*)targetTls->StartAddressOfRawData, reinterpret_cast<void*>(sourceTls->StartAddressOfRawData),
               sourceTls->EndAddressOfRawData - sourceTls->StartAddressOfRawData);
    }

    // copy over the offset to the new imports directory
    DWORD oldProtect;
    VirtualProtect(sourceNtHeader, 0x1000, PAGE_EXECUTE_READWRITE, &oldProtect);

    sourceNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT] =
        ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

    memcpy(sourceNtHeader, ntHeader,
           sizeof(IMAGE_NT_HEADERS) + (ntHeader->FileHeader.NumberOfSections * (sizeof(IMAGE_SECTION_HEADER))));
}

void GameLoader::Execute()
{
    auto start = static_cast<void (*)()>(m_pEntryPoint);
    return start();
}
