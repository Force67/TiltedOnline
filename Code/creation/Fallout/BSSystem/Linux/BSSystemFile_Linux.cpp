#ifdef __linux__

#include "BSSystem/BSSystemFile.h"
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

namespace creation
{
    extern int errno;

    namespace
    {
        BSSystemFile::Error TranslateError(int linuxError)
        {
            switch (linuxError)
            {
            // Linux has no invalid path error
            //case ERROR_PATH_NOT_FOUND:
                //return BSSystemFile::Error::kInvalidPath;
            case ENOENT:
                return BSSystemFile::Error::kInvalid;
            case EEXIST:
                return BSSystemFile::Error::kFileError;
            case EPERM:
            case EACCES:
                return BSSystemFile::Error::kMemoryError;
            case EINVAL:
                return BSSystemFile::Error::kInvalidParam;
            default:
                return BSSystemFile::Error::kBusy;
            }
        }
    }

    BSSystemFile::BSSystemFile() : m_flags(1), m_fileDescriptor(-1)
    {}

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
        close(m_fileDescriptor);
    }

    BSSystemFile::Error BSSystemFile::OpenImpl(const char* path, AccessMode accessMode, OpenMode openMode)
    {
        int32_t fileFlags;

        switch (accessMode)
        {
        case AccessMode::kReadonly:
            fileFlags = O_RDONLY;
            break;
        case AccessMode::kWriteonly:
            fileFlags = O_WRONLY;
            break;
        case AccessMode::kReadWrite:
            fileFlags = O_RDWR;
            break;
        default:
            fileFlags = O_RDONLY;
            break;
        }

        switch (openMode)
        {
        case OpenMode::kCreate:
            fileFlags |= O_CREAT;
            break;
        case OpenMode::kTruncate:
            fileFlags |= O_TRUNC;
            break;
        case OpenMode::kExclude:
            fileFlags |= O_EXCL;
            break;
        case OpenMode::kNone:
        case OpenMode::kAppend:
        default:
            fileFlags |= O_APPEND;
            break;
        }

        // TODO: figure out exact flag combinations
        uint32_t flagAttributes = 0x68000000;
        if ((!m_flags & 0x80000000))
        {
            if (m_flags & 0x1Eu)
                flagAttributes = 0x28000000;
        }
 
        m_fileDescriptor = open(path, fileFlags);

        // if we fail, try again in create mode
        if (m_fileDescriptor == -1 && openMode == OpenMode::kAppend)
        {
            m_fileDescriptor = open(path, fileFlags | O_CREAT);
        }

        if (m_fileDescriptor == -1)
        {
            return TranslateError(errno);
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
        // file is busy
        if (m_flags & 0x80000000)
            return Error::kBusy;

        if (read(m_fileDescriptor, buffer, length) == -1)
        {
            return TranslateError(errno);
        }

        return Error::kSuccess;
    }

    BSSystemFile::Error BSSystemFile::WriteImpl(const void* buffer, size_t length, size_t* numWritten)
    {
         // file is busy
        if (m_flags & 0x80000000)
            return Error::kBusy;

        if (write(m_fileDescriptor, buffer, length) == -1) 
        {
            return TranslateError(errno);
        }

        return Error::kSuccess;
    }

    BSSystemFile::Error BSSystemFile::GetSizeImpl(size_t *size)
    {
        struct stat s;
        if (fstat(m_fileDescriptor, &s) == -1)
        {
            *size = 0;
            return Error::kBusy;
        }

        *size = static_cast<size_t>(s.st_size);
        return BSSystemFile::kSuccess;
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
        if (m_fileDescriptor != -1)
            Close();

        m_flags = rhs.m_flags;
        m_fileDescriptor = rhs.m_fileDescriptor;

        rhs.m_flags = 1;
        rhs.m_fileDescriptor = -1;
        return m_fileDescriptor != -1;
    }

    BSSystemFile::Error BSSystemFile::SeekImpl(size_t offset, int whence, size_t* pos)
    {
        *pos = 0;

        // file is busy
        if (m_flags & 0x80000000)
            return Error::kBusy;

        auto seekResult = lseek64(m_fileDescriptor, static_cast<long int>(offset), whence);
        if (seekResult == -1)
            return TranslateError(errno);
        
        *pos = seekResult;
        return Error::kSuccess;
    }

    BSSystemFile::Error BSSystemFile::Seek(size_t offset, int whence, size_t* pos)
    {
        Error result = SeekImpl(offset, whence, pos);
        m_flags &= 0x80000000;
        m_flags |= result;
        return result;
    }
}

#endif