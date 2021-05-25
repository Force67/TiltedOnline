#pragma once

#include "TESFileRecord.h"
#include <cstdint>

#include "BSCore/BSTArray.h"
#include "BSCore/BSList.h"
#include "BSCore/BSString.h"

#include "FileIO/BSFile.h"

namespace creation
{
    struct BSSystemFile__Info
    {
        uint64_t x;
        uint64_t ModifyTime;
        uint64_t CreateTime;
        /* 0x0018 */ uint64_t uiFileSize;
    }; /* size: 0x0020 */

    class TESFile
    {
    public:
        TESFile();

        __int64 LoadTESInfo(TESFile* self, TESFile* apParentFile);

    private:
        void ClearMasterList();

        bool ReadFormHeader();
        bool ReadChunkHeader();

        char pad0[0x3c0];
    };

    static_assert(sizeof(TESFile) == 0x3C0);

} // namespace creation
