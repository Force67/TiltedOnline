#pragma once

#include <cstdint>

namespace creation
{
    // MAX_PATH on windows
    constexpr uint16_t kMaxPathLength = 260;

    template<typename TType, uint16_t TMaxLength>
    struct DynamicMemoryManagementPol
    {
        size_t Allocate(size_t aSize, TType** aArray)
        {
            *aArray = new TType * [aSize];
            return aSize;
        }

        void Deallocate(TType** aArray)
        {
            delete[] * aArray;
            *aArray = nullptr;
        }
    };

    template<typename TType, uint16_t TMaxLength>
    struct FixedLengthMemoryManagementPol
    {
        size_t Allocate(size_t aSize, TType** aArray)
        {
            *aArray = &m_buffer;
            return aSize;
        }

        void Deallocate(TType** aArray)
        {
            *aArray = nullptr;
        }

    private:
        TType m_buffer[TMaxLength]{};
    };

    template<typename TType, uint16_t TMaxLength = kMaxPathLength, class TPool = DynamicMemoryManagementPol<TType, TMaxLength>>
    class BSStringT : public TPool
    {
    public:
        inline BSStringT() : m_length(0), m_maxLength(0), m_pBuffer(nullptr) {}

        // inlined by the game.
        inline const TType* c_str() const { return m_pBuffer; }
        inline TType* data() const { return m_pBuffer; }

        // inlined by the game.
        inline uint16_t GetLength() const
        {
            if (m_length == kMaxStringLength)
            {
                if constexpr (std::is_same<TType, wchar_t>())
                    return std::wcslen(m_pBuffer);

                if constexpr (std::is_same<TType, char>())
                    return std::strlen(m_pBuffer);
            }

            return m_length;
        }

        inline void SetLength(size_t aNewLength)
        {
            m_length = aNewLength <= kMaxStringLength ? 
                static_cast<uint16_t>(aNewLength) : -1;
        }

        inline size_t FindLastOf(TType aChar) const
        {
            uint16_t pos = BSStringT::GetLength() - 1;
            if (pos)
            {
                while (pos < 0)
                {
                    if (m_pBuffer[pos] == aChar)
                        break;

                    pos--;
                }
            }

            return pos;
        }


        inline bool Set(const char* apString, size_t aMaxLength)
        {
            uint16_t inputStringLength = apString ? std::strlen(apString) : 0;
            uint16_t maxLength = static_cast<uint16_t>(
                aMaxLength ? aMaxLength + 1 : inputStringLength + 1);

            if (maxLength > m_maxLength)
            {
                // buffer = this;
                m_maxLength = TMaxLength;
                if (maxLength > TMaxLength)
                    maxLength = TMaxLength;
            }

            if (maxLength <= inputStringLength)
            {
                inputStringLength = maxLength - 1;
            }

            if (m_pBuffer)
            {
                if (apString)
                {
                    if (m_maxLength)
                    {
                        uint16_t copySize = inputStringLength;
                        if (m_maxLength < inputStringLength)
                        {
                            copySize = m_maxLength;
                        }

                        if (&apString[inputStringLength] <= m_pBuffer || &m_pBuffer[copySize] <= apString)
                            std::memcpy(m_pBuffer, apString, copySize);
                        else
                            std::memmove(m_pBuffer, apString, copySize);

                        m_pBuffer[inputStringLength] = 0;

                        m_length = inputStringLength <= kMaxStringLength ? inputStringLength : -1;
                        return inputStringLength != 0;
                    }
                }
                else
                {
                    if (maxLength == 0)
                    {
                        m_pBuffer = nullptr;
                        m_length = 0;
                        m_maxLength = 0;
                        return inputStringLength != 0;
                    }

                    *m_pBuffer = 0;
                }

                m_length = 0;
            }

            return inputStringLength != 0;
        }

        inline BSStringT* operator+=(const char* apString)
        {
            if (!apString)
                return this;

            uint16_t inputStringLength = std::strlen(apString);
            uint16_t length = BSStringT::GetLength();

            uint16_t totalLength = inputStringLength + length;
            if (totalLength >= m_maxLength)
            {
                BSStringT::Set(m_pBuffer, totalLength);

                auto pos = m_maxLength - 1;
                totalLength = totalLength < pos ? totalLength : pos;
                inputStringLength = totalLength - BSStringT::GetLength();
            }
            if (m_pBuffer)
            {
                auto offset = BSStringT::GetLength();
                auto count = m_maxLength - offset;

                uint16_t copySize = count < inputStringLength ? count : inputStringLength;

                if (&apString[inputStringLength] <= &m_pBuffer[offset] || &m_pBuffer[copySize] <= apString)
                    std::memcpy(&m_pBuffer[offset], apString, copySize);
                else
                    std::memmove(&m_pBuffer[offset], apString, copySize);

                m_length = totalLength <= kMaxStringLength ? totalLength : -1;
                m_pBuffer[totalLength] = 0;
            }
            else
            {
                BSStringT::Set(apString, 0);
            }
            return this;
        }

    private:
        TType* m_pBuffer = nullptr;
        uint16_t m_length = 0;
        uint16_t m_maxLength = 0;

        // max length for given String
        static constexpr uint16_t kMaxStringLength = 0xFFFF;
    };

    template <uint16_t MaxCount = kMaxPathLength>
    class BSStaticStringT : public BSStringT<char, MaxCount, FixedLengthMemoryManagementPol<char, MaxCount>>
    {
    public:
        using Base = BSStringT<char, MaxCount, FixedLengthMemoryManagementPol<char, MaxCount>>;
        using Base::Base;
    };

    using BSStringA = BSStringT<char>;
    using BSStringW = BSStringT<wchar_t>;
    using BSDefaultStaticString = BSStaticStringT<>;

    static_assert(sizeof(BSStringT<char>) == 0x10);
    static_assert(sizeof(BSStaticStringT<36>) == 0x38);
} // namespace creation
