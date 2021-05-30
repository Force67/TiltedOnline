#pragma once

#include <cstdint>

class Atmosphere;
class Stars;
class Sun;
class Clouds;
class Moon;
class Precipitation;
class SkyEffectController;

class TESClimate;
class TESWeather;
class TESRegion;

class BSMultiboundNode;

class Sky
{
public:
    static Sky* Get() noexcept;

    virtual ~Sky() = default;

    enum class SkyMode : uint32_t
    {
        kNone,
        kInterior,
        kSkydomeOnly,
        kFullSky
    };

    void Reset();
    void SetSkyMode(SkyMode aIndex);

  public:
    char pad_0x0000[0x8];             // 0x0000
    BSMultiboundNode* multiBoundNode; // 0x0008
    char pad_0x0010[0x8];             // 0x0010
    float N0000003C;                  // 0x0018
    char pad_0x001C[0xC];             // 0x001C
    void* N0000003A;             // 0x0028
    float N00000039;                  // 0x0030
    float N00000455;                  // 0x0034
    float N00000038;                  // 0x0038
    DWORD N0000045E;                  // 0x003C
    TESClimate* climate;              // 0x0040
    TESWeather* weather;              // 0x0048
    TESWeather* outgoingWeather;      // 0x0050
    TESWeather* incomingWeather;      // 0x0058
    TESWeather* unkWeatherCycle;      // 0x0060
    TESRegion* region;                // 0x0068
    Atmosphere* atomosphere;          // 0x0070
    Stars* stars;                     // 0x0078
    Sun* sun;                         // 0x0080
    Clouds* clouds;                   // 0x0088
    Moon* masser;                // 0x0090
    Moon* secunda;                    // 0x0098
    Precipitation* precipitation;     // 0x00A0
    float N0000004A;                  // 0x00A8
    float N000004B2;                  // 0x00AC
    float N00000049;                  // 0x00B0
    char pad_0x00B4[0x18];            // 0x00B4
    void* pUnk;                  // 0x00CC
    char pad_0x00D4[0x4];             // 0x00D4
    float N00000054;                  // 0x00D8
    char pad_0x00DC[0xC];             // 0x00DC

    // there is likely some sort of matrices below
    float N00000052;                  // 0x00E8
    float N000004DC;                  // 0x00EC
    float N00000051;                  // 0x00F0
    float N000004C1;                  // 0x00F4
    float N0000005F;                  // 0x00F8
    char pad_0x00FC[0x4];             // 0x00FC
    float N0000005E;                  // 0x0100
    float N000004A5;                  // 0x0104
    float N0000005D;                  // 0x0108
    char pad_0x010C[0x4];             // 0x010C
    float N0000005C;                  // 0x0110
    char pad_0x0114[0x24];            // 0x0114
    double N00000050;                 // 0x0138
    float defaultLight;               // 0x0140
    char pad_0x0144[0x30];            // 0x0144
    float color1;                     // 0x0174
    float color2;                     // 0x0178
    float color3;                     // 0x017C
    char pad_0x0180[0xC];             // 0x0180
    float windSpeed;                  // 0x018C
    float windMult;                   // 0x0190
    float fogSpeed;                   // 0x0194
    float fogMult;                    // 0x0198
    char pad_0x019C[0x4];             // 0x019C
    float N0000006B;                  // 0x01A0
    float unk;                        // 0x01A4
    float unk1;                       // 0x01A8
    float N00000476;                  // 0x01AC
    float weatherPercentBase;         // 0x01B0
    float weatherPercentTop;          // 0x01B4
    float currentWeatherPrecent;      // 0x01B8
    __int32 skyMode;                  // 0x01BC
    char pad_0x01C0[0x8];             // 0x01C0
    float flashDuration;              // 0x01C8
    DWORD N00000466;                  // 0x01CC
    char pad_0x01D0[0x4];             // 0x01D0
    float secondsBetweenWindowUpdate; // 0x01D4
    float nextWeatherpercent;         // 0x01D8
    DWORD overrideFlags;              // 0x01DC
    char pad_0x01E0[0x78];            // 0x01E0
    float auroraFadeInStart;          // 0x0258
    float autoraFadeIn;               // 0x025C
    float auroraFadeOutStart;         // 0x0260
    float autoraFadeOut;              // 0x0264
    DWORD64 N000001EF;                // 0x0268
    char pad_0x0270[0x8];             // 0x0270
    SkyEffectController* skyEffectController; // 0x0278
    char pad_0x0280[0x8];             // 0x0280
};

//static_assert(sizeof(Sky) == 712);
