#pragma once

namespace creation
{
    class BSFixedString;

    namespace BSScript
    {
        class Object;
        class Variable;
        class Statement;
        class IObjectHandlePolicy;

        class IVirtualMachine
        {
            virtual ~IVirtualMachine() = 0;

            virtual void sub_01();
            virtual void sub_02();
            virtual void sub_03();
            virtual void sub_04();
            virtual void sub_05();
            virtual void sub_06();
            virtual void sub_07();
            virtual void sub_08();
            virtual void sub_09();
            virtual void sub_0A();
            virtual void sub_0B();
            virtual void sub_0C();
            virtual void sub_0D();
            virtual void sub_0E();
            virtual void sub_0F();
            virtual void sub_10();
            virtual void sub_11();
            virtual void sub_12();
            virtual void sub_13();
            virtual void sub_14();
            virtual void sub_15();
            virtual void sub_16();
            virtual void sub_17();
            virtual void sub_18();
            virtual void sub_19();
            virtual void sub_1A();
            virtual void sub_1B();
            virtual void sub_1C();
            virtual void sub_1D();
            virtual void sub_1E();
            virtual void sub_1F();
            virtual void sub_20();
            virtual void sub_21();
            virtual void sub_22();
            virtual void sub_23();
            virtual void sub_24();
            virtual bool GetVariable(BSScript::Object** aObject, uint32_t aIndex, BSScript::Variable* aVariable);
            virtual void sub_26();
            virtual void sub_27();
            virtual void sub_28();
            virtual void sub_29();
            virtual void sub_2A();
            virtual void sub_2B();
            virtual void sub_2C();
            virtual void SendEvent(uint64_t aId, const BSFixedString& acEventName,
                                   std::function<bool(Statement*)>& aFunctor) const noexcept;
            virtual void sub_2E();
            virtual void sub_2F();
            virtual void sub_30();
            virtual void sub_31();
            virtual void sub_32();
            virtual IObjectHandlePolicy* GetObjectHandlePolicy();
        };
    }
}
