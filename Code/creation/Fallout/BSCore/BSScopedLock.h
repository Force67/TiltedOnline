#pragma once

namespace creation
{
    template<class T>
    class BSScopedLock
    {
    public:
        BSScopedLock(T& aLock) : m_lock(aLock) { m_lock.Lock(); }
        ~BSScopedLock() { m_lock.Unlock(); }

    private:
        T& m_lock;
    };
}
