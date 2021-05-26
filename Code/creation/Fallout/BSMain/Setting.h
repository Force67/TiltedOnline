#pragma once

#include <cstdint>

namespace creation
{
    struct Setting
    {
        virtual ~Setting() = default;
        virtual bool IsPrefSetting() { return false; }

        union
        {
            float f;
            int i;
            uint32_t u;
            bool b;
            char c;
            const char* str;
        } value;

        int asInt() const { return value.i; }
        float AsFloat() const { return value.f; }
        bool asBool() const { return value.b; }

        const char* pKey;
    };

    struct GameSettingCollection;
    struct INISettingCollection;

    template<typename T>
    struct SettingT : Setting
    {
    };

    using GameSetting = SettingT<GameSettingCollection>;
    using INISetting = SettingT<INISettingCollection>;
}
