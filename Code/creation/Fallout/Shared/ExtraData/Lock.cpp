#include "Lock.h"

namespace creation
{
    void Lock::SetLock(bool aIsLocked) noexcept
    {
        TP_THIS_FUNCTION(TSetLock, void, Lock, bool);
        POINTER_FALLOUT4(TSetLock, realSetLock, 0x1400B7E10 - 0x140000000);

        return ThisCall(realSetLock, this, aIsLocked);
    }
}
