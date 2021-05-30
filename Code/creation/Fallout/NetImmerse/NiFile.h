#pragma once

#include "BSSystem/BSSystemFile.h"
#include "NetImmerse/NiBinaryStream.h"

namespace creation
{
class NiFile : public NiBinaryStream
{
    friend class BSSystemFile;

  public:
    enum OpenMode
    {
        kReadOnly = 0,
        kWriteOnly = 1,
        kAppend = 2,
        kReadWrite = 3,
    };

    enum SeekMode
    {
        kSet,
        kCur,
        kEnd
    };

    explicit NiFile(const char* fileName, OpenMode mode, size_t bufferSize);
    virtual ~NiFile() = default;

    operator bool() override
    {
        return good;
    }

    void Seek(int byteCount) override;
    void GetBufferInfo(BufferInfo* info) override;

    virtual void Seek(int offset, int whence);
    virtual size_t GetFileSize();

    bool Flush();
    size_t Read(void* buffer, size_t length) override;
    size_t Write(const void* buffer, size_t length) override;

  private:
    size_t ReadImpl(void* buffer, size_t length);
    size_t WriteImpl(const void* buffer, size_t length);

  protected:
    size_t bufferAllocSize = 0;
    size_t bufferSize = 0;
    size_t bufferPos = 0;
    size_t currentPos = 0;
    char* buffer = nullptr;
    BSSystemFile file;
    OpenMode openMode{OpenMode::kReadOnly};
    bool good = false;
};

static_assert(sizeof(NiFile) == 0x50);
} // namespace creation
