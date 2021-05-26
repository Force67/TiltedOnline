
#include "TESFile.h"

namespace creation
{
    TESFile::TESFile()
    {
    }

    bool TESFile::ReadFormHeader()
    {
#if 0
        if (file->Read(currentForm) != sizeof(FormHeader))
        {
            currentForm = {};
            return false;
        }


        // impelement type check.
#endif
        return true;
    }

    bool TESFile::ReadChunkHeader()
    {
        return false;
    }

    __int64 TESFile::LoadTESInfo(TESFile* self, TESFile* apParentFile)
    {
        ClearMasterList();
        return 0;
    }
} // namespace creation
