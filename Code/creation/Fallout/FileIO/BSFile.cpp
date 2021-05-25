
#include "FileIO/BSFile.h"
#include <io.h>

namespace creation
{
BSFile::BSFile(const char* fileNameIn, NiFile::OpenMode mode, size_t bufferSize, bool bunk)
    : NiFile(fileNameIn, mode, bufferSize)
{
    if (std::strlen(fileNameIn) <= 260)
        std::strncpy(fileName, fileNameIn, sizeof(fileName));
    // game just sets filename to zero otherwise, should really be a bug check tho
    else
        std::memset(fileName, 0, sizeof(fileName));

    if (((mode - 1) & 0xFFFFFFFD) == 0)
    {
        Open(false, bunk);
    }

    if (mode == OpenMode::kReadOnly)
    {
        if (file.UnkFlagBits() == 0 || file.UnkFlagBits() == 3)
        {
            good = true;
        }
        else
        {
            good = Exist();
        }
    }
}

size_t BSFile::Read(void* buffer, size_t length)
{
    if (file.UnkFlagBits() != 0 && file.UnkFlagBits() != 3)
        Open(false, false);

    size_t result = 0;

    if (m_bunk9 || m_pCache)
    {
        // ASSERT
        __debugbreak();
    }
    else
    {
        result += NiFile::Read(buffer, length);
    }

    m_bsFilePos += result;
    return result;
}

size_t BSFile::Write(const void* buffer, size_t length)
{
    size_t result = NiFile::Write(buffer, length);
    m_bsFilePos += result;
    return result;
}

void BSFile::Seek(int offset, int whence)
{
    size_t cursor = static_cast<size_t>(offset);
    size_t truePos = 0;
    size_t size = 0;

    switch (whence)
    {
    case SeekMode::kSet: {
        truePos = cursor;
        if (openMode == OpenMode::kReadOnly)
        {
            whence = SeekMode::kCur;
            cursor = offset - m_bsFilePos;
        }
        break;
    }
    case SeekMode::kCur: {
        truePos = cursor + m_bsFilePos;
        break;
    }
    case SeekMode::kEnd: {
        size = GetSize();
        truePos = size - cursor;
        break;
    }
    default: {
        truePos = m_bsFilePos;
        break;
    }
    }

    if (whence == SeekMode::kSet)
    {
        truePos = cursor;
        if (openMode == OpenMode::kReadOnly)
        {
            whence = SeekMode::kCur;
            cursor = offset - m_bsFilePos;
        }
    }

    if (truePos != m_bsFilePos)
    {
        m_bsFilePos = truePos;
        if (m_bunk9 && !m_pCache)
        {
            NiFile::Seek(cursor, whence);
            return;
        }
        if (whence == SeekMode::kCur)
        {
            size_t pos = bufferPos + cursor;
            if ((bufferPos + cursor) >= 0 && pos < bufferSize)
            {
                absolutePos += cursor;
                bufferPos = pos;
                return;
            }

            currentPos += cursor + bufferPos - bufferSize;
        }
        else
        {
            currentPos = SeekMode::kSet ? cursor : size - cursor;
        }

        NiFile::Flush();
    }
}

size_t BSFile::GetFileSize()
{
    if (m_fileSize)
        return m_fileSize;

    GetSize();
    return m_fileSize;
}

bool BSFile::Open(bool unk1, bool unk2)
{
    // both parameters remain unused.
    if (file.UnkFlagBits() == 0 || file.UnkFlagBits() == 3)
        return true;

    bufferSize = 0;
    bufferPos = 0;

    // release previous cache.
    if (buffer)
    {
        // TODO: niFree
        free(buffer);
    }

    BSSystemFile::AccessMode amode;
    BSSystemFile::OpenMode omode{BSSystemFile::OpenMode::kNone};

    switch (openMode)
    {
    default:
    case OpenMode::kReadOnly:
        amode = BSSystemFile::AccessMode::kReadonly;
        break;
    case OpenMode::kWriteOnly:
        omode = BSSystemFile::OpenMode::kCreate;
        amode = BSSystemFile::AccessMode::kWriteonly;
        break;
    case OpenMode::kAppend:
        amode = BSSystemFile::AccessMode::kReadWrite;
        break;
    case OpenMode::kReadWrite:
        amode = BSSystemFile::AccessMode::kReadWrite;
        omode = BSSystemFile::OpenMode::kCreate;
        break;
    }

    BSSystemFile osFile(fileName, amode, omode, false, 0, false);
    good = osFile.UnkFlagBits() != 0;

    // move file handle
    if (good)
        file = osFile;

    return good;
}

bool BSFile::OpenByFilePointer(BSSystemFile* apFile)
{
    // is file already opened?
    if (file.UnkFlagBits() == 0 || file.UnkFlagBits() == 3)
        return true;

    bufferPos = 0;
    bufferSize = 0;

    // TODO: nibuffer.
    if (buffer)
        free(buffer);

    file = *apFile;

    // TODO: BsFile::CheckIsGood();

    return good;
}

size_t BSFile::GetSize()
{
    // reset and gather again.
    m_fileSize = 0;

    if (((openMode - 1) & 0xFFFFFFFD) == 0)
        NiFile::Flush();

    BSFile::Open(false, false);

    if (file.UnkFlagBits() == 0 || file.UnkFlagBits() == 3)
        m_fileSize = NiFile::GetFileSize();
    return m_fileSize;
}

bool BSFile::Exist()
{
    if (file.UnkFlagBits() == 0 || file.UnkFlagBits() == 3)
        return true;

    return _access(fileName, 0) != -1;
}

size_t BSFile::ReadStringA(void* arString, size_t auiMaxLength)
{
    __debugbreak();
    return 0;
}

size_t BSFile::ReadStringW(wchar_t* arString, size_t auiMaxLength)
{
    __debugbreak();
    return 0;
}

size_t BSFile::GetLine(char* pBuffer, size_t uiMaxBytes, wchar_t cMark)
{
    if (uiMaxBytes <= 1)
    {
        *pBuffer = 0;
        return 0;
    }

    size_t length = 0;
    char character = 0;
    int offset = 0;

    for (int i = 1; i < uiMaxBytes; ++i)
    {
        size_t count = Read(&character, sizeof(character));
        absolutePos += count;
        length += count;
        if (count != 1)
            break;

        if (character == cMark)
            break;

        pBuffer[i - 1] = character;
        ++offset;
    }
    pBuffer[offset] = 0;
    return length;
}

size_t BSFile::WriteStringA(BSStringA* string, bool binary)
{
    uint16_t length = string->GetLength();

    // NOT COMPLETE, the game sets to some global variable.
    // TODO

    size_t result = Write(string->c_str(), length + binary);
    currentPos += result;
    m_bsFilePos += result;
    return result;
}

size_t BSFile::WriteStringW(BSStringW* string, bool binary)
{
    uint16_t length = string->GetLength();

    // NOT COMPLETE, the game sets to some global variable.
    // TODO

    size_t result = Write(string->c_str(), length + binary);
    currentPos += result;
    m_bsFilePos += result;
    return result;
}
} // namespace creation
