#pragma once

namespace creation
{
    class Lock
    {
    public:
        void SetLock(bool aIsLocked) noexcept;

        uint8_t m_uiLockLevel;
        uint8_t pad1[7];
        void* unk8;
        uint8_t m_uiFlags;
        uint8_t pad11[3];
        uint32_t m_uiNumberOfTries;
    };

    static_assert(sizeof(Lock) == 0x18);
}
