#pragma once

namespace creation
{
    template <class TValue>
    class BSTSingletonImplicit
    {
    public:
        using ValueType = TValue;
    };

    template <class TValue>
    class BSTSingletonExplicit
    {
    public:
        using ValueType = TValue;
    };

    template <class TValue>
    struct BSTSingletonSDMOpStaticBuffer
    {
    public:
        using ValueType = TValue;
    };

    template <class Traits>
    struct BSTSingletonSDMBase :
        public Traits,
        public BSTSingletonSDMOpStaticBuffer<typename Traits::ValueType>
    {
    public:
    };

    template <class TValue, class Buffer>
    struct BSTSDMTraits
    {
    public:
        using ValueType = TValue;
    };

    template <class TValue, template <class> class Buffer = BSTSingletonSDMOpStaticBuffer>
    struct BSTSingletonSDM :
        public BSTSingletonSDMBase<BSTSDMTraits<T, Buffer<T>>>
    {
    public:
        using ValueType = TValue;

        static TValue* GetInstance() { return s_mpInstance; }
        static operator bool() { return s_mpInstance != nullptr; }

    private:
        static TValue* s_mpInstance;
    };

    //static_assert(sizeof(BSTSingletonSDM<void>) == 8);
}
