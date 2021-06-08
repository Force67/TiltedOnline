#pragma once

#include "BSCore/BSString.h"
#include "NetImmerse/NiFile.h"

namespace creation
{
    class BSFile : public NiFile
    {
    public:
        explicit BSFile(const char* fileName, NiFile::OpenMode mode, size_t bufferSize, bool bunk);
        virtual ~BSFile() = default;

        size_t Read(void* buffer, size_t length) override;
        size_t Write(const void* buffer, size_t length) override;
        void Seek(int offset, int whence) override;
        size_t GetFileSize() override;

        virtual bool Open(bool unk, bool unk2);
        virtual bool OpenByFilePointer(BSSystemFile* apFile);
        virtual size_t GetSize();
        virtual size_t ReadStringA(void* arString, size_t auiMaxLength);
        virtual size_t ReadStringW(wchar_t* arString, size_t auiMaxLength);
        virtual size_t GetLine(char* pBuffer, size_t uiMaxBytes, wchar_t cMark);
        virtual size_t WriteStringA(BSStringA* string, bool binary);
        virtual size_t WriteStringW(BSStringW* string, bool binary);
        virtual bool Exist();

        const char* GetFileName() const
        {
            return fileName;
        }

        // inlined by the game
        template <typename T> size_t Read(T& item)
        {
            size_t c = Read(reinterpret_cast<void*>(&item), sizeof(T));
            absolutePos += c;
            return c;
        }

        template<typename T> size_t ReadN(T* buffer, size_t n)
        {
            size_t c = Read(reinterpret_cast<void*>(buffer), n);
            absolutePos += c;
            return c;
        }

    private:
        bool m_bunk0 = false;
        char* m_bsFileBuffer = nullptr;
        int m_iunk1 = -1;
        uint32_t m_iunk2 = 0;
        uint32_t m_iunk3 = 0;
        char fileName[260]{ 0 };
        size_t m_lastResult = 0;
        size_t m_ioSize = 0;
        size_t m_bsFilePos = 0;
        size_t m_fileSize = 0;
        bool m_bunk4 = false;
        void* m_pCache = nullptr;
        uint32_t m_Id = -1;
        uint32_t m_priority = 0;
        uint8_t m_bunk8 = 0;
        bool m_bunk9 = false;
    };

    static_assert(sizeof(BSFile) == 0x1B0);
} // namespace creation
