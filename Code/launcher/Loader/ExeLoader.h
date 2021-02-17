#pragma once

/*
 * This file is part of the CitizenFX project - http://citizen.re/
 *
 * See LICENSE and MENTIONS in the root of the source tree for information
 * regarding licensing.
 */

#include <Windows.h>
#include <TiltedCore/Filesystem.hpp>

class ExeLoader
{
  public:
    using entrypoint_t = void(*)();
    using funchandler_t = FARPROC(*)(HMODULE, const char*);

    explicit ExeLoader(uintptr_t aLoadLimit, funchandler_t aFuncPtr);

    bool Load(std::filesystem::path& source);
    entrypoint_t GetEntryPoint() const;

  private:
    void LoadSections(const IMAGE_NT_HEADERS* apNtHeader);
    void LoadImports(const IMAGE_NT_HEADERS* apNtHeader);
    void LoadTLS(const IMAGE_NT_HEADERS* apNtHeader, const IMAGE_NT_HEADERS* apSourceNt);

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
    const funchandler_t m_pFuncHandler = nullptr;

    uintptr_t m_loadLimit;
    HMODULE m_moduleHandle = nullptr;
    void* m_pEntryPoint = nullptr;
};
