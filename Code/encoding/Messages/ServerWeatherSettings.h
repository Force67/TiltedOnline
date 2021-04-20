#pragma once

#include "Message.h"
#include <Structs/Objects.h>
#include <Structs/GameId.h>

struct ServerWeatherSettings final : ServerMessage
{
    ServerWeatherSettings() : ServerMessage(kServerWeatherSettings)
    {
    }

    void SerializeRaw(TiltedPhoques::Buffer::Writer& aWriter) const noexcept override;
    void DeserializeRaw(TiltedPhoques::Buffer::Reader& aReader) noexcept override;

    // TODO: compare all
    bool operator==(const ServerWeatherSettings& acRhs) const noexcept
    {
        return PercentBase == acRhs.PercentBase && 
               PercentTop == acRhs.PercentTop && 
               CurrentPercent == acRhs.CurrentPercent && 
               NextPercent == acRhs.NextPercent &&
               GetOpcode() == acRhs.GetOpcode();
    }


    float PercentBase;
    float PercentTop;
    float CurrentPercent;
    float NextPercent;
    float ColorR;
    float ColorG;
    float ColorB;
    float unk1;
    float unk2;
    int OverrideFlags;
    int SkyMode;

    GameId climateId;
    GameId weatherId;
    GameId outgoingWeatherId;
    GameId incomingWeatherId;
    GameId nextWeatherId;
    GameId regionId;
};
