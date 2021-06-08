#pragma once

#include "BSScript/BSScriptIVirtualMachine.h"

namespace creation
{
    class BSScript::IVirtualMachine;
    class GameVM
    {
        virtual ~GameVM();

        static GameVM* Get();

        uint8_t pad8[0xB0 - 0x8];
        BSScript::IVirtualMachine* m_pVirtualMachine;
        uint8_t padB8[0x87A8 - 0xB8];
    };

    static_assert(sizeof(GameVM) == 0x87A8);
}
