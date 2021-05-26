
#include "BSSystem/BSSystemFile.h"
#include <Windows.h>

namespace creation
{
    namespace
    {
        BSSystemFile::Error TranslateError(DWORD winError)
        {
            switch (winError)
            {
            case ERROR_SUCCESS:
                return BSSystemFile::Error::kSuccess;
            case ERROR_FILE_NOT_FOUND:
                return BSSystemFile::Error::kInvalidPath;
            case ERROR_PATH_NOT_FOUND:
                return BSSystemFile::Error::kInvalid;
            case ERROR_WRITE_PROTECT:
                return BSSystemFile::Error::kMemoryError;
            case ERROR_ALREADY_EXISTS:
                return BSSystemFile::Error::kFileError;
            default:
                return BSSystemFile::Error::kBusy;
            }
        }
    } // namespace

    BSSystemFile::BSSystemFile() : m_flags(1), m_pHandle(INVALID_HANDLE_VALUE)
    {
    }

    BSSystemFile::BSSystemFile(const char* name, AccessMode amode, OpenMode omode, bool async, uint32_t sizehint, bool unk8)
    {
        // IRL that'd be an union
        m_flags = (unk8 | (2 * async)) << 30;

        Error result = OpenImpl(name, amode, omode);
        m_flags &= 0x80000000;
        m_flags |= result;
    }

    BSSystemFile::~BSSystemFile()
    {
        Close();
    }

    void BSSystemFile::Close()
    {
        ::CloseHandle(m_pHandle);
    }

    BSSystemFile::Error BSSystemFile::OpenImpl(const char* path, AccessMode accessMode, OpenMode openMode)
    {
        DWORD desiredAccess;
        DWORD creationDisposition;

        switch (accessMode)
        {
        case AccessMode::kReadonly:
            desiredAccess = GENERIC_READ;
            break;
        case AccessMode::kWriteonly:
            desiredAccess = GENERIC_WRITE;
            break;
        case AccessMode::kReadWrite:
            desiredAccess = GENERIC_WRITE | GENERIC_READ;
            break;
        default:
            desiredAccess = GENERIC_READ;
            break;
        }

        switch (openMode)
        {
        case OpenMode::kCreate:
            creationDisposition = CREATE_ALWAYS;
            break;
        case OpenMode::kTruncate:
            creationDisposition = TRUNCATE_EXISTING;
            break;
        case OpenMode::kExclude:
            creationDisposition = CREATE_NEW;
            break;
        case OpenMode::kNone:
        case OpenMode::kAppend:
        default:
            creationDisposition = OPEN_EXISTING;
            break;
        }

        // TODO: figure out exact flag combinations
        DWORD flagAttributes = 0x68000000;
        if ((!m_flags & 0x80000000))
        {
            if (m_flags & 0x1Eu)
                flagAttributes = 0x28000000;
        }

        m_pHandle =
            ::CreateFileA(path, desiredAccess, FILE_SHARE_READ, nullptr, creationDisposition, flagAttributes, nullptr);

        // if we fail, try again in create mode
        if (m_pHandle == INVALID_HANDLE_VALUE && openMode == OpenMode::kAppend)
        {
            m_pHandle =
                ::CreateFileA(path, desiredAccess, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, flagAttributes, nullptr);
        }

        if (m_pHandle == INVALID_HANDLE_VALUE)
        {
            return TranslateError(::GetLastError());
        }

        // if we are not busy.
        if (openMode == OpenMode::kAppend && (!m_flags & 0x80000000))
        {
            LARGE_INTEGER position{ 0 };
            if (!SetFilePointerEx(m_pHandle, position, &position, FILE_END))
            {
                return TranslateError(::GetLastError());
            }
        }

        return Error::kSuccess;
    }

    BSSystemFile::Error BSSystemFile::Read(void* buffer, size_t length, size_t* numRead)
    {
        Error result = ReadImpl(buffer, length, numRead);
        m_flags &= 0x80000000; // we are busy now
        m_flags |= result;
        return result;
    }

    BSSystemFile::Error BSSystemFile::Write(const void* buffer, size_t length, size_t* numWritten)
    {
        Error result = WriteImpl(buffer, length, numWritten);
        m_flags &= 0x80000000; // we are busy now
        m_flags |= result;
        return result;
    }

    BSSystemFile::Error BSSystemFile::ReadImpl(void* buffer, size_t length, size_t* numRead)
    {
        *numRead = 0u;

        // file is busy
        if (m_flags & 0x80000000)
            return Error::kBusy;

        if (::ReadFile(m_pHandle, buffer, static_cast<DWORD>(length), reinterpret_cast<LPDWORD>(numRead), nullptr))
        {
            return Error::kSuccess;
        }

        return TranslateError(::GetLastError());
    }

    BSSystemFile::Error BSSystemFile::WriteImpl(const void* buffer, size_t length, size_t* numWritten)
    {
        *numWritten = 0u;

        // file is busy
        if (m_flags & 0x80000000)
            return Error::kBusy;

        if (::WriteFile(m_pHandle, buffer, static_cast<DWORD>(length), reinterpret_cast<LPDWORD>(numWritten), nullptr))
        {
            return Error::kSuccess;
        }

        return TranslateError(::GetLastError());
    }

    BSSystemFile::Error BSSystemFile::GetSizeImpl(size_t* size)
    {
        LARGE_INTEGER FileSize;
        if (::GetFileSizeEx(m_pHandle, &FileSize))
        {
            *size = FileSize.QuadPart;
            return BSSystemFile::kSuccess;
        }

        *size = 0;
        return Error::kBusy;
    }

    BSSystemFile::Error BSSystemFile::GetSize(size_t* size)
    {
        Error result = GetSizeImpl(size);
        m_flags &= 0x80000000;
        m_flags |= result;
        return result;
    }

    bool BSSystemFile::operator=(BSSystemFile& rhs)
    {
        if (m_pHandle != INVALID_HANDLE_VALUE)
            ::CloseHandle(m_pHandle);

        m_flags = rhs.m_flags;
        m_pHandle = rhs.m_pHandle;

        rhs.m_flags = 1;
        rhs.m_pHandle = INVALID_HANDLE_VALUE;
        return m_pHandle != INVALID_HANDLE_VALUE;
    }

    BSSystemFile::Error BSSystemFile::SeekImpl(size_t offset, int whence, size_t* pos)
    {
        *pos = 0;

        // file is busy
        if (m_flags & 0x80000000)
            return Error::kBusy;

        LARGE_INTEGER distance;
        distance.QuadPart = offset;

        LARGE_INTEGER newPointer{};

        if (::SetFilePointerEx(m_pHandle, distance, &newPointer, static_cast<DWORD>(whence)))
        {
            *pos = newPointer.QuadPart;
            return Error::kSuccess;
        }

        return TranslateError(::GetLastError());
    }

    BSSystemFile::Error BSSystemFile::Seek(size_t offset, int whence, size_t* pos)
    {
        Error result = SeekImpl(offset, whence, pos);
        m_flags &= 0x80000000;
        m_flags |= result;
        return result;
    }
} // namespace creation
