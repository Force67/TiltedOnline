#pragma once

#include "Shared/TESForms/FormComponents/TESForm.h"
#include "Shared/ExtraData/ExtraDataList.h"

namespace creation
{
    class TESObjectREFR : public TESForm
    {
        TESObjectCELL* m_pParentCell;
        OBJ_REFR m_data;
        LOADED_REF_DATA* m_pLoadedData;
        BGSInventoryList* m_pInventory;
        void* m_pExtraData; // WS
        unsigned __int16 m_usReferenceScale;
        unsigned __int8 m_eModelState;
        bool m_bDestroying;
    };
}
