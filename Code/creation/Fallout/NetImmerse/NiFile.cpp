
#include "NetImmerse/NiFile.h"

namespace creation
{
NiFile::NiFile(const char* fileName, OpenMode mode, size_t bufferSize) : openMode(mode), bufferAllocSize(bufferSize)
{
    BSSystemFile::AccessMode accessMode;
    BSSystemFile::OpenMode openMode{BSSystemFile::OpenMode::kNone};

    switch (mode)
    {
    default:
    case OpenMode::kReadOnly:
        accessMode = BSSystemFile::AccessMode::kReadonly;
        break;
    case OpenMode::kWriteOnly:
        openMode = BSSystemFile::OpenMode::kCreate;
        accessMode = BSSystemFile::AccessMode::kWriteonly;
        break;
    case OpenMode::kAppend:
        accessMode = BSSystemFile::AccessMode::kReadWrite;
        break;
    case OpenMode::kReadWrite:
        accessMode = BSSystemFile::AccessMode::kReadWrite;
        openMode = BSSystemFile::OpenMode::kCreate;
        break;
    }

    BSSystemFile osFile(fileName, accessMode, openMode, false, 0, false);
    good = osFile.UnkFlagBits() != 0;

    // move file handle
    if (good)
        file = osFile;

    if (good && bufferSize)
        buffer = reinterpret_cast<char*>(malloc(bufferSize)); // NI ALLOC
}

// TODO: free in dtor

void NiFile::Seek(int count)
{
    Seek(count, SeekMode::kCur);
}

void NiFile::Seek(int offset, int whence)
{
    if (!good)
        return;

    if (whence == SeekMode::kCur)
    {
        size_t totalPos = bufferPos + offset;
        if ((bufferPos + offset) >= 0 && totalPos < bufferSize)
        {
            bufferPos = totalPos;
            absolutePos += offset;
            return;
        }

        if (openMode == OpenMode::kReadOnly)
            offset = bufferPos - bufferSize + offset;
    }

    Flush();

    size_t setPos = 0;
    auto result = file.Seek(offset, whence, &setPos);
    good = result == BSSystemFile::Error::kSuccess;

    if (good)
    {
        currentPos = setPos;
        absolutePos = setPos;
    }
}

void NiFile::GetBufferInfo(BufferInfo* info)
{
    info->buffer = buffer;
    info->size = GetFileSize();
    info->allocsz = bufferAllocSize;
    info->unk4 = bufferSize;
    info->bufPos = bufferPos;
    info->position = GetPosition();
}

bool NiFile::Flush()
{
    if (openMode == OpenMode::kReadOnly)
    {
        bufferSize = 0;
        bufferPos = 0;
        return true;
    }

    if (bufferPos == 0)
        return true;

    size_t read = 0;
    file.Read(buffer, bufferPos, &read);

    if (read == bufferPos)
    {
        bufferPos = 0;
        return true;
    }

    good = false;
    return false;
}

size_t NiFile::ReadImpl(void* buffer, size_t length)
{
    size_t count = 0;
    file.Read(buffer, length, &count);

    currentPos += count;
    return count;
}

size_t NiFile::WriteImpl(const void* buffer, size_t length)
{
    size_t count = 0;
    file.Write(buffer, length, &count);
    return count;
}

size_t NiFile::Read(void* readBuffer, size_t readLength)
{
    if (!good)
        return 0;

    size_t count = bufferSize - bufferPos;
    size_t offset = 0;

    char* bufferPointer = buffer;

    if (readLength <= count)
    {
        std::memcpy(bufferPointer, &buffer[bufferPos], readLength);
        bufferPos += readLength;
        return readLength;
    }

    if (count)
    {
        std::memcpy(readBuffer, &buffer[bufferPos], count);
        bufferPointer += count;
        readLength -= count;
        offset = count;
    }

    // write contents to disk.
    Flush();

    if (readLength <= bufferAllocSize)
    {
        bufferSize = ReadImpl(buffer, bufferAllocSize);
        if (bufferSize < readLength)
            readLength = bufferSize;

        std::memcpy(bufferPointer, &buffer[bufferPos], readLength);
        bufferPos += readLength;
        return offset + readLength;
    }

    return offset + ReadImpl(bufferPointer, readLength);
}

size_t NiFile::Write(const void* writeBuffer, size_t writeLength)
{
    if (!good)
        return 0;

    size_t count = bufferAllocSize - bufferPos;
    size_t offset = 0;

    // modern c++ is fun!
    char* bufferPointer = static_cast<char*>(const_cast<void*>(writeBuffer));

    if (writeLength <= count)
    {
        std::memcpy(buffer, writeBuffer, writeLength);
        bufferPos += writeLength;
        return offset + writeLength;
    }

    if (count)
    {
        std::memcpy(&buffer[bufferPos], buffer, count);
        bufferPointer += count;
        writeLength -= count;
        offset = count;
        bufferPos = bufferAllocSize;
    }

    if (Flush())
    {
        if (writeLength >= bufferAllocSize)
            return offset + WriteImpl(bufferPointer, writeLength);

        std::memcpy(buffer, bufferPointer, writeLength);
        bufferPos += writeLength;
        return offset + writeLength;
    }

    return 0;
}

size_t NiFile::GetFileSize()
{
    size_t count = 0;
    file.GetSize(&count);
    return count;
}
} // namespace creation
