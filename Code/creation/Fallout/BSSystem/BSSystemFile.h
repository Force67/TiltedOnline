#pragma once

#include <cstdint>

namespace creation
{
    class BSSystemFile
    {
    public:
        enum AccessMode
        {
            kReadonly,
            kReadWrite,
            kWriteonly,
        };

        enum OpenMode
        {
            kNone,
            kAppend,
            kCreate,
            kTruncate,
            kExclude
        };

        enum Error
        {
            kSuccess,
            kInvalidFile,
            kInvalidPath,
            kFileError,
            kInvalid,
            kMemoryError,
            kBusy,
            kInvalidParam,
            kUnsupported
        };

        BSSystemFile();
        ~BSSystemFile();

        // Opens the file directly.
        explicit BSSystemFile(const char* name, AccessMode amode, OpenMode omode, bool async, uint32_t sizehint, bool unk8);

        // Move opened file.
        bool operator=(BSSystemFile& rhs);

        void Close();

        inline uint32_t UnkFlagBits()
        {
            return m_flags & 0x3FFFFFFF;
        }

        Error Write(const void* buffer, size_t length, size_t* numWritten);
        Error Read(void* buffer, size_t length, size_t* numRead);
        Error GetSize(size_t* size);
        Error Seek(size_t offset, int whence, size_t* pos);

    private:
        Error OpenImpl(const char* path, AccessMode accessMode, OpenMode openMode);
        Error ReadImpl(void* buffer, size_t length, size_t* numRead);
        Error WriteImpl(const void* buffer, size_t length, size_t* numWritten);

        Error GetSizeImpl(size_t* size);
        Error SeekImpl(size_t offset, int whence, size_t* pos);

    private:
        uint32_t m_flags;
        void* m_pHandle;
    };

    static_assert(sizeof(BSSystemFile) == 0x10);
} // namespace creation
