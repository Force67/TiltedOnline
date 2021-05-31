#pragma once

#include "TESActorBase.h"
#include "Shared/FormComponents/TESRaceForm.h"
#include "Shared/FormComponents/BGSOverridePackCollection.h"
#include "Shared/FormComponents/BGSForcedLocRefType.h"
#include "Shared/FormComponents/BGSNativeTerminalForm.h"
#include "Shared/Events/BSTEventSink.h"
#include "Shared/FormComponents/BGSAttachParentArray.h"
#include "BSCore/BSTArray.h"
#include "BGSCharacterTint.h"

namespace creation
{
    class BGSColorForm;
    class BGSTextureSet;
    class TESClass;
    class TESCombatStyle;
    class BGSOutfit;
    class BGSHeadPart;

    class TESNPC : public TESActorBase, public TESRaceForm, public BGSOverridePackCollection,
                   public BGSForcedLocRefType, public BGSNativeTerminalForm,
                   public BSTEventSink<MenuOpenCloseEvent>
    {
        static constexpr uint32_t Type = FormType::kNPC;

        static TESNPC* Create(const String& acBuffer, uint32_t aChangeFlags) noexcept;

        struct Head
        {
            BGSColorForm* hairColor;
            void* unk8;
            BGSTextureSet* textures;
        };

        BGSAttachParentArray m_attachParentArray;
        uint32_t unk238; // 238 - set to 0
        uint16_t unk23C; // 23C - set to 0
        uint8_t unk23E; // 23E - set to 1
        uint8_t pad23F; // 23F
        TESClass* npcClass; // 240
        Head* head; // 248 - set to nullptr
        void* unk250; // 250
        TESCombatStyle* combatStyle; // 258
        uint8_t unk260[8]; // 260
        void* unk268; // 268
        void* unk270; // 270
        float smallWeight; // 278
        float muscularWeight; // 27C
        float largeWeight; // 280
        float unk284; // 284
        float unk288; // 288
        uint32_t pad28C; // 28C
        void* unk290; // 290
        void* unk298; // 298
        void* unk2A0; // 2A0 - set to nullptr
        void* unk2A8; // 2A8
        BGSOutfit* outfits[2]; // 2B0
        void* unk2C0; // 2C0
        TESFaction* faction; // 2C8
        BGSHeadPart** headParts; // 2D0
        void* unk2D8; // 2D8
        void* morphRegionData; // 2E0
        uint8_t headPartCount; // 2E8
        uint8_t unk2E9; // 2E9
        uint8_t skinColor[4]; // 2EA
        uint16_t pad2EE; // 2EE
        uint64_t unk2F0; // 2F0 - not sure about type
        void* unk2F8; // 2F8
        BSTArray<BGSCharacterTint::Entry*>* tints; // 300

        BGSHeadPart* GetHeadPart(uint32_t aType);
        void Serialize(String* apSaveBuffer) const noexcept;
        void Deserialize(const String& acBuffer, uint32_t aChangeFlags) noexcept;
        void Initialize() noexcept;
    };

    static_assert(sizeof(TESNPC) == 0x308);
}
