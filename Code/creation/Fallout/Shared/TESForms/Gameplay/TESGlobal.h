#pragma once

#include "BSCore/BSString.h"
#include "Shared/FormComponents/TESForm.h"

namespace creation
{
    class TESGlobal final : public TESForm
    {
    public:
        TESGlobal();

        inline void SetValue(float aVal) { value = aVal; }
        inline const float GetValue() const { return value; }

    private:
        BSStringA m_editorId;

    public:
        // the game technically uses setters and getters everywhere, but this gets messy quickly,
        // so we just expose the direct value
        float value = 0.f;
    };

    static_assert(sizeof(TESGlobal) == 0x38);
}
