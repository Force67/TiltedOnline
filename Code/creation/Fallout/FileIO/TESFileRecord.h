#pragma once

#include "TESFileRecordTypes.inl"
#include <cstdint>

#pragma pack(push, 1)

struct FormHeader
{
    enum FLAGS
    {
        kMasterFile = 1,
        kCompressed = 0x40000,
        kIgnored = 0x1000,
        kIsMarker = 0x800000,
    };

    FormEnum form;
    uint32_t length;
    uint32_t flags;
    uint32_t formId;
    uint32_t vcsVersion;
    uint16_t formVersion;
    uint16_t unk8;

    // use these instead of comparing the bit flags directly
    // it avoids bugs and aids portability.
    bool Compressed() const
    {
        return (flags & FLAGS::kCompressed) != 0;
    }

    bool Ignored() const
    {
        return (flags & FLAGS::kIgnored) != 0;
    }

    bool Master() const
    {
        return (flags & FLAGS::kMasterFile) != 0;
    }

    bool DefaultForm() const
    {
        return formId - 1 <= 0x7FE;
    }
};

struct ChunkRecord
{
    ChunkId id;
    uint16_t size;
};

struct FILE_HEADER
{
    /* 0x0000 */ float fVersion;
    /* 0x0004 */ uint32_t iFormCount;
    /* 0x0008 */ uint32_t iNextFormID;
}; /* size: 0x000c */

#pragma pack(pop)

static_assert(sizeof(FormHeader) == 24);
static_assert(sizeof(ChunkRecord) == 6);
static_assert(sizeof(FILE_HEADER) == 12);
