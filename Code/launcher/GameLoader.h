#pragma once

/*
 * This file is part of the CitizenFX project - http://citizen.re/
 *
 * See LICENSE and MENTIONS in the root of the source tree for information
 * regarding licensing.
 */

#include <Windows.h>
#include <TiltedCore/Filesystem.hpp>

class GameLoader
{
  public:
    explicit GameLoader(uintptr_t aLoadLimit);

    void Load(std::filesystem::path& source);
    void Execute();

  private:
    void LoadSections(const IMAGE_NT_HEADERS* apNtHeader);
    void LoadImports(const IMAGE_NT_HEADERS* apNtHeader);

    template <typename T> inline const T* GetRVA(uint32_t rva)
    {
        return (T*)(m_pBinary + rva);
    }

    template <typename T> inline T* GetTargetRVA(uint32_t rva)
    {
        return (T*)((uint8_t*)m_moduleHandle + rva);
    }

  private:
    const uint8_t* m_pBinary = nullptr;
    uintptr_t m_loadLimit;
    HMODULE m_moduleHandle = nullptr;
    void* m_pEntryPoint = nullptr;
};
