#pragma once

namespace creation
{
    class TESForm;

    class BaseFormComponent
    {
    public:
        virtual ~BaseFormComponent() = default;

        virtual int GetFormComponentType() { return 0; }
        virtual void InitializeDataComponent() {}
        virtual void ClearDataComponent() {}
        virtual void InitComponent(TESForm* apForm) {}
        virtual void CopyComponent(BaseFormComponent* apBase, TESForm* apForm) {}
        virtual void CopyComponent(BaseFormComponent* apBase) {}
    };
}
