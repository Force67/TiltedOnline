#pragma once

#include <cstdint>
#include <memory>

namespace creation
{
class NiBinaryStream
{
  public:
    struct BufferInfo
    {
        void* buffer;
        size_t size;
        size_t allocsz;
        size_t unk4;
        size_t bufPos;
        size_t position;
    };

    virtual ~NiBinaryStream() = default;

    virtual operator bool() = 0;
    virtual void Seek(int byteCount) = 0;

    virtual size_t GetPosition()
    {
        return absolutePos;
    }

    virtual void GetBufferInfo(BufferInfo* info)
    {
        std::memset(info, 0, sizeof(BufferInfo));
    }

    virtual size_t Read(void* buffer, size_t length) = 0;
    virtual size_t Write(const void* buffer, size_t length) = 0;

  protected:
    uint64_t absolutePos;
};

static_assert(sizeof(NiBinaryStream) == 0x10);
} // namespace creation
