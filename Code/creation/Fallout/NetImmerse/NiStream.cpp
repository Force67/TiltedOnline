
#include "NetImmerse/NiStream.h"

namespace creation
{
    NiStream::NiStream()
    {
    }


    bool NiStream::Load(NiBinaryStream* pkIstr)
    {
        return false;
    }

    bool NiStream::Load(char* pcBuffer, int iBufferSize)
    {
        return false;
    }

    bool NiStream::Load(const char* pcFileName)
    {
        return false;
    }

    bool NiStream::Save(NiBinaryStream* pkOstr)
    {
        return false;
    }

    bool NiStream::Save(char*& pcBuffer, int& iBufferSize)
    {
        return false;
    }

    bool NiStream::Save(const char* pcFileName)
    {
        return false;
    }

    bool NiStream::RegisterFixedString(const BSFixedString& kString)
    {
        return false;
    }

    bool NiStream::RegisterSaveObject(NiObject* pkObject)
    {
        return false;
    }

    void NiStream::ChangeObject(NiObject* pkNewObject)
    {

    }

    uint32_t NiStream::GetLinkIDFromObject(const NiObject* pkObject) const
    {
        return 0;
    }

    void NiStream::SaveLinkID(const NiObject* pkObject)
    {

    }

    bool NiStream::LoadHeader()
    {
        return false;
    }

    void NiStream::SaveHeader()
    {

    }

    bool NiStream::LoadStream()
    {
        return false;
    }

    bool NiStream::SaveStream()
    {
        return false;
    }

    void NiStream::RegisterObjects()
    {

    }

    void NiStream::LoadTopLevelObjects()
    {

    }

    void NiStream::SaveTopLevelObjects()
    {

    }

    uint32_t NiStream::PreSaveObjectSizeTable()
    {
        return 0;
    }

    bool NiStream::SaveObjectSizeTable(uint32_t uiStartOffset)
    {
        return false;
    }

    bool NiStream::LoadObjectSizeTable()
    {
        return false;
    }

} // namespace creation
