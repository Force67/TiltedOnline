#pragma once

namespace creation
{
    class MiddleProcess;

    class AIProcess
    {
    public:
        uint8_t unk0[0x8];
        MiddleProcess* m_pMiddleProcess;
        uint8_t unk10[0xD8];
    };

    static_assert(offsetof(AIProcess, m_pMiddleProcess) == 0x8);
    static_assert(sizeof(AIProcess) == 0xE8);
}
