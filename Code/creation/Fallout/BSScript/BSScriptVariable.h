#pragma once

namespace creation
{
    namespace BSScript
    {
        class Variable
        {
            Variable() = default;
            ~Variable() noexcept;

            void Reset() noexcept;
            void Clear() noexcept;

            template <class T> void Set(T aValue) noexcept
            {
                static_assert(false);
            }

            enum Type : uint64_t
            {
                kEmpty,
                kHandle,
                kString,
                kInteger,
                kFloat,
                kBoolean,
                kVariable,
                kStruct
            };

            union Data 
            {
                int32_t i;
                const char* s;
                float f;
                bool b;
            };

            Type type;
            Data data;
        };
    }

    template <> void BSScript::Variable::Set(int32_t aValue) noexcept;
    template <> void BSScript::Variable::Set(float aValue) noexcept;
    template <> void BSScript::Variable::Set(bool aValue) noexcept;
    template <> void BSScript::Variable::Set(const char* acpValue) noexcept;
}
