#pragma once

#include <atomic>

#include "BSCore/BSFixedString.h"

namespace creation
{
    class NiBinaryStream;
    class NiObject;

    class NiStream
    {
    public:
        NiStream();
        virtual ~NiStream() = default;

        virtual bool Load(NiBinaryStream* pkIstr);
        virtual bool Load(char* pcBuffer, int iBufferSize);
        virtual bool Load(const char* pcFileName);

        virtual bool Save(NiBinaryStream* pkOstr);
        virtual bool Save(char*& pcBuffer, int& iBufferSize);
        virtual bool Save(const char* pcFileName);

        virtual bool RegisterFixedString(const BSFixedString& kString);
        virtual bool RegisterSaveObject(NiObject* pkObject);
        virtual void ChangeObject(NiObject* pkNewObject);
        virtual uint32_t GetLinkIDFromObject(const NiObject* pkObject) const;
        virtual void SaveLinkID(const NiObject* pkObject);

    protected:
        virtual bool LoadHeader();
        virtual void SaveHeader();
        virtual bool LoadStream();
        virtual bool SaveStream();
        virtual void RegisterObjects();
        virtual void LoadTopLevelObjects();
        virtual void SaveTopLevelObjects();
        virtual uint32_t PreSaveObjectSizeTable();
        virtual bool SaveObjectSizeTable(uint32_t uiStartOffset);
        virtual bool LoadObjectSizeTable();

    private:
    };

} // namespace creation
