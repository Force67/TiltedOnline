#pragma once

#include <Sky/Moon.h>
#include <NetImmerse/NiPointer.h>

class NiNode;
class BSMultiBoundNode;

class TESClimate;
class TESWeather;
class TESRegion;
class TESImageSpace;

class Atmosphere;
class Stars;
class Sun;
class Clouds;
class Moon;
class Precipitation;
class SkyEffectController;

class ReferenceEffect;
class ImageSpaceModifierInstanceForm;


struct NiColorA
{
    /* 0x0000 */ float r;
    /* 0x0004 */ float g;
    /* 0x0008 */ float b;
    /* 0x000c */ float a;
};

struct NiColor
{
    /* 0x0000 */ float r;
    /* 0x0004 */ float g;
    /* 0x0008 */ float b;
};

class Sky
{
public:

};

