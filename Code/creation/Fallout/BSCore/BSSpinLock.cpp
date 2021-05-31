#include "BSSpinLock.h"

namespace creation
{
    void BSSpinLock::Lock() noexcept
    {
        TP_THIS_FUNCTION(TLock, void, BSSpinLock);
        POINTER_FALLOUT4(TLock, s_lock, 0x1400244D0 - 0x140000000);

        s_lock.Get()(this);
    }

    void BSSpinLock::Unlock() noexcept
    {
        TP_THIS_FUNCTION(TUnlock, void, BSSpinLock);
        POINTER_FALLOUT4(TUnlock, s_unlock, 0x140026AA0 - 0x140000000);

        s_unlock.Get()(this);
    }
} // namespace creation
