/*******************************************************************************
 * @file   ZipFile.cpp
 * @author Brian Hoffpauir
 * @date   12.28.2024
 * @brief  API for working with ZIP archive files.
 *
 * Copyright (c) 2024, Brian Hoffpauir All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/
#include "Engine/EngineStd.hpp"
#include "Resources/ZipFile.hpp"

#include <cctype>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>

#include <zlib.h>

#if BGE_PLATFORM_LINUX
#include <linux/limits.h>
static constexpr std::size_t kPATH_MAX = PATH_MAX;
#else
static constexpr std::size_t kPATH_MAX = 4'096;
#endif

namespace
{
	bool OpenFile(FILE** ppFile, const std::wstring& resFileName, const char* pMode)
	{
#ifdef BGE_PLATFORM_WIN
		// Convert wide string to UTF-8 on Windows
		int utf8Length = WideCharToMultiByte(CP_UTF8, 0, resFileName.c_str(), -1, nullptr, 0, nullptr, nullptr);
		if (utf8Length <= 0)
			return false;

		std::string utf8FileName(utf8Length, '\0');
		WideCharToMultiByte(CP_UTF8, 0, resFileName.c_str(), -1, utf8FileName.data(), utf8Length, nullptr, nullptr);

		//*ppFile = std::fopen(utf8FileName.c_str(), pMode);
		_wfopen_s(ppFile, resFileName.c_str(), L"rb");
#else
		// Convert wide string to UTF-8 for non-Windows platforms
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		std::string utf8FileName = converter.to_bytes(resFileName);

		*ppFile = std::fopen(utf8FileName.c_str(), pMode);
#endif
		return (*ppFile != nullptr);
	}
} // End namespace

namespace BGE
{
	/*
	 * For information regarding the ZIP archive file format visit:
	 * https://en.wikipedia.org/wiki/ZIP_(file_format)
	 */
	// Basic types.
	using dword = std::uint32_t; // unsigned int;
	using word = std::uint16_t; // unsigned short;
	using byte = std::uint8_t; // unsigned char;
	// ZIP file structures. NOTE: These have to be packed.
#pragma pack(1)
	struct ZipFile::TZipLocalHeader
	{
		static constexpr dword kSIGNATURE = 0x04034b50;
		dword sig; //!< Local file header signature = 0x04034b50
		word  version; //!< Version needed to extract (minimum)
		word  flag; //!< General purpose bit flag
		word  compression; //!< Z_NO_COMPRESSION or Z_DEFLATED
		word  modTime; //!< File last modification time
		word  modDate; //!< File last modification date
		dword crc32; //!< CRC-32 of uncompressed data
		dword cSize; //!< Compressed size (or 0xffffffff for ZIP64)
		dword ucSize; //!< Uncompressed size (or 0xffffffff for ZIP64)
		word  fnameLen; //!< File name length (n)
		word  xtraLen;  //!< Extra field length (m)
		// File name follows the end of the header
		// Extra field length follows file name
	};

	struct ZipFile::TZipDirHeader
	{
		static constexpr dword kSIGNATURE = 0x06054b50;
		dword sig; //!< End of central directory signature = 0x06054b50
		word  nDisk; //!< Number of this disk (or 0xffff for ZIP64)
		word  nStartDisk; //!< Disk where central directory starts (or 0xffff for ZIP64)
		word  nDirEntries; //!< Number of central directory records on this disk (or 0xffff for ZIP64)
		word  totalDirEntries; //!< Total number of central directory records (or 0xffff for ZIP64)
		dword dirSize; //!< Size of central directory (bytes) (or 0xffffffff for ZIP64)
		dword dirOffset; //!< Offset of start of central directory, relative to start of archive (or 0xffffffff for ZIP64)
		word  cmntLen; //!< Comment length (n)
	};

	struct ZipFile::TZipDirFileHeader
	{
		static constexpr dword kSIGNATURE = 0x02014b50;
		dword sig; //!< Central directory file header signature = 0x02014b50
		word  verMade; //!< Version made by
		word  verNeeded; //!< Version needed to extract (minimum)
		word  flag; //!< General purpose bit flag
		word  compression; //!< Compression method COMP_xxxx
		word  modTime; //!< File last modification time
		word  modDate; //!< File last modification date
		dword crc32; //!< CRC-32 of uncompressed data
		dword cSize; //!< Compressed size (or 0xffffffff for ZIP64)
		dword ucSize; //!< Uncompressed size (or 0xffffffff for ZIP64)
		word  fnameLen; //!< File name length (n)
		word  xtraLen; //!< Extra field length (m)
		word  cmntLen; //!< File comment length (k)
		word  diskStart; //!< Disk number where file starts (or 0xffff for ZIP64)
		word  intAttr; //!< Internal file attributes
		dword extAttr; //!< External file attributes
		dword hdrOffset; //!< Relative offset of local file header (or 0xffffffff for ZIP64)
		// File name follows header.
		// Extra field follows file name.
		// File comment follows extra field.
		char *GetName   (void) const { return (char *)(this + 1);   }
		char *GetExtra  (void) const { return GetName() + fnameLen; }
		char *GetComment(void) const { return GetExtra() + xtraLen; }
	};
#pragma pack()

	ZipFile::ZipFile(void)
	{
		// Static assertions for ZipFile struct byte sizes.
		BGE_STATIC_ASSERT(sizeof(ZipFile::TZipDirHeader) == 22);
		BGE_STATIC_ASSERT(sizeof(ZipFile::TZipDirFileHeader) == 46);
		BGE_STATIC_ASSERT(sizeof(ZipFile::TZipLocalHeader) == 30);

		m_nEntries=0; m_pFile=NULL; m_pDirData=NULL;
	}

	ZipFile::~ZipFile(void)
	{
		End();
		fclose(m_pFile);
	}

	/*
	 * Function:   Init
	 * Purpose:    Initialize the object and read the zip file directory.
	 * Parameters: A stdio FILE* used for reading.
	 */
	bool ZipFile::Init(const std::wstring &resFileName)
	{
		End();

		//_wfopen_s(&m_pFile, resFileName.c_str(), _T("rb"));
		OpenFile(&m_pFile, resFileName, "rb");
		if (!m_pFile)
		{
			return false;
		}
		// Assuming no extra comment at the end, read the whole end record.
		TZipDirHeader dh;

		std::fseek(m_pFile, -(int)sizeof(dh), SEEK_END);
		long dhOffset = std::ftell(m_pFile);
		std::memset(&dh, 0, sizeof(dh));
		std::fread(&dh, sizeof(dh), 1, m_pFile);

		BGE_LOG("Resources", "sizeof(TZipDirHeader) = %d bytes", sizeof(TZipDirHeader));
		BGE_LOG("Resources", "sizeof(TZipDirFileHeader) = %d bytes", sizeof(TZipDirFileHeader));
		BGE_LOG("Resources", "sizeof(TZipLocalHeader) = %d bytes", sizeof(TZipLocalHeader));
		BGE_LOG("Resources", "dg.sig = 0x%X | TZipDirHeader::kSIGNATURE = 0x%X", dh.sig, TZipDirHeader::kSIGNATURE);
		// Check
		if (dh.sig != TZipDirHeader::kSIGNATURE)
		{
			return false;
		}
		// Go to the beginning of the directory.
		std::fseek(m_pFile, dhOffset - dh.dirSize, SEEK_SET);

		// Allocate the data buffer, and read the whole thing.
		m_pDirData = BGE_NEW char[dh.dirSize + dh.nDirEntries * sizeof(*m_papDir)];
		if (!m_pDirData)
		{
			return false;
		}
		std::memset(m_pDirData, 0, dh.dirSize + dh.nDirEntries * sizeof(*m_papDir));
		std::fread(m_pDirData, dh.dirSize, 1, m_pFile);

		// Now process each entry.
		char *pfh = m_pDirData;
		m_papDir = (const TZipDirFileHeader **)(m_pDirData + dh.dirSize);

		bool bSuccess = true;

		for (int i = 0; i < dh.nDirEntries && bSuccess; ++i)
		{
			TZipDirFileHeader &fh = *(TZipDirFileHeader*)pfh;

			// Store the address of nth file for quicker access.
			m_papDir[i] = &fh;

			BGE_LOG("Resources", "fh.sig = 0x%X | TZipDirFileHeader::kSIGNATURE = 0x%X", fh.sig, TZipDirFileHeader::kSIGNATURE);
			// Check the directory entry integrity.
			if (fh.sig != TZipDirFileHeader::kSIGNATURE)
				bSuccess = false;
			else
			{
				pfh += sizeof(fh);

				// Convert UNIX slashes to DOS backlashes.
				for (int j = 0; j < fh.fnameLen; j++)
				{
					if (pfh[j] == '/')
						pfh[j] = '\\';
				}
				char fileName[kPATH_MAX];
				std::memcpy(fileName, pfh, fh.fnameLen);
				fileName[fh.fnameLen]=0;
				//_strlwr_s(fileName, kPATH_MAX);
				std::string spath = StringToLower(fileName);
				m_ZipContentsMap[spath] = i;

				// Skip name, extra and comment fields.
				pfh += fh.fnameLen + fh.xtraLen + fh.cmntLen;
			}
		}

		if (!bSuccess)
		{
			BGE_SAFE_DELETE_ARRAY(m_pDirData);
		}
		else
		{
			m_nEntries = dh.nDirEntries;
		}
		return bSuccess;
	}

	ZipFileIndex ZipFile::Find(const std::string &path) const
	{
		std::string lowerCase = BGE::StringToLower(path);
		ZipContentsMap::const_iterator it = m_ZipContentsMap.find(lowerCase);
		if (it == m_ZipContentsMap.end())
		{
			return -1;
		}

		return it->second;
	}

	/*
	* Function:   End
	* Purpose:    Finish the object
	* Parameters:
	*/
	void ZipFile::End()
	{
		m_ZipContentsMap.clear();
		BGE_SAFE_DELETE_ARRAY(m_pDirData);
		m_nEntries = 0;
	}

	/*
	 * Function:   GetFileName
	 * Purpose:    Return the name of a file
	 * Parameters: The file index and the buffer where to store the filename
	 */
	std::string ZipFile::GetFileName(ZipFileIndex index)  const
	{
		std::string fileName = "";
		if (index >= 0 && index < m_nEntries)
		{
			char pszDest[kPATH_MAX];
			std::memcpy(pszDest, m_papDir[index]->GetName(), m_papDir[index]->fnameLen);
			pszDest[m_papDir[index]->fnameLen] = '\0';
			fileName = pszDest;
		}
		return fileName;
	}

	/*
	* Function:   GetFileLen
	* Purpose:    Return the length of a file so a buffer can be allocated
	* Parameters: The file index.
	*/
	std::size_t ZipFile::GetFileLen(ZipFileIndex index) const
	{
		if (index < 0 || index >= m_nEntries)
		{
			return 0;
		}
		else
		{
			return m_papDir[index]->ucSize;
		}
	}

	/*
	* Function:   ReadFile
	* Purpose:    Uncompress a complete file
	* Parameters: The file index and the pre-allocated buffer
	*/
	bool ZipFile::ReadFile(ZipFileIndex index, void *pBuf)
	{
		if (pBuf == nullptr || index < 0 || index >= m_nEntries)
		{
			return false;
		}
		// Quick'n dirty read, the whole file at once.
		// Ungood if the ZIP has huge files inside

		// Go to the actual file and read the local header.
		std::fseek(m_pFile, m_papDir[index]->hdrOffset, SEEK_SET);
		TZipLocalHeader h;

		std::memset(&h, 0, sizeof(h));
		std::fread(&h, sizeof(h), 1, m_pFile);
		BGE_LOG("Resources", "h.sig = 0x%X | TZipLocalHeader::kSIGNATURE = 0x%X", h.sig, TZipLocalHeader::kSIGNATURE);
		if (h.sig != TZipLocalHeader::kSIGNATURE)
		{
			return false;
		}
		// Skip extra fields
		std::fseek(m_pFile, h.fnameLen + h.xtraLen, SEEK_CUR);

		if (h.compression == Z_NO_COMPRESSION)
		{
			// Simply read in raw stored data.
			std::fread(pBuf, h.cSize, 1, m_pFile);
			return true;
		}
		else if (h.compression != Z_DEFLATED)
		{
			return false;
		}
		// Alloc compressed data buffer and read the whole stream
		char *pcData = BGE_NEW char[h.cSize];
		if (!pcData)
		{
			return false;
		}
		std::memset(pcData, 0, h.cSize);
		std::fread(pcData, h.cSize, 1, m_pFile);

		bool bRet = true;

		// Setup the inflate stream.
		z_stream stream;
		int err;

		stream.next_in = (Bytef*)pcData;
		stream.avail_in = (uInt)h.cSize;
		stream.next_out = (Bytef*)pBuf;
		stream.avail_out = h.ucSize;
		stream.zalloc = (alloc_func)0;
		stream.zfree = (free_func)0;

		// Perform inflation. wbits < 0 indicates no zlib header inside the data.
		err = inflateInit2(&stream, -MAX_WBITS);
		if (err == Z_OK)
		{
			err = inflate(&stream, Z_FINISH);
			inflateEnd(&stream);
			if (err == Z_STREAM_END)
				err = Z_OK;
			inflateEnd(&stream);
		}
		if (err != Z_OK)
		{
			bRet = false;
		}
		BGE_SAFE_DELETE_ARRAY(pcData);
		return bRet;
	}

	/*
	 * Function:      ReadLargeFile
	 * Purpose:       Uncompress a complete file with callbacks.
	 * Parameters:    The file index and the pre-allocated buffer
	 */
	bool ZipFile::ReadLargeFile(ZipFileIndex index, void *pBuf, ZipProgressCallback progressCallback)
	{
		if (pBuf == nullptr || index < 0 || index >= m_nEntries)
		{
			return false;
		}
		// Quick'n dirty read, the whole file at once.
		// Ungood if the ZIP has huge files inside

		// Go to the actual file and read the local header.
		std::fseek(m_pFile, m_papDir[index]->hdrOffset, SEEK_SET);
		TZipLocalHeader h;

		std::memset(&h, 0, sizeof(h));
		std::fread(&h, sizeof(h), 1, m_pFile);
		if (h.sig != TZipLocalHeader::kSIGNATURE)
		{
			return false;
		}
		// Skip extra fields
		std::fseek(m_pFile, h.fnameLen + h.xtraLen, SEEK_CUR);

		if (h.compression == Z_NO_COMPRESSION)
		{
			// Simply read in raw stored data.
			std::fread(pBuf, h.cSize, 1, m_pFile);
			return true;
		}
		else if (h.compression != Z_DEFLATED)
		{
			return false;
		}
		// Alloc compressed data buffer and read the whole stream
		char *pcData = BGE_NEW char[h.cSize];
		if (!pcData)
		{
			return false;
		}
		std::memset(pcData, 0, h.cSize);
		std::fread(pcData, h.cSize, 1, m_pFile);

		bool bRet = true;

		// Setup the inflate stream.
		z_stream stream;
		int err;

		stream.next_in = (Bytef*)pcData;
		stream.avail_in = (uInt)h.cSize;
		stream.next_out = (Bytef*)pBuf;
		stream.avail_out = (128 * 1024); //  read 128k at a time h.ucSize;
		stream.zalloc = (alloc_func)0;
		stream.zfree = (free_func)0;

		// Perform inflation. wbits < 0 indicates no zlib header inside the data.
		err = inflateInit2(&stream, -MAX_WBITS);
		if (err == Z_OK)
		{
			uInt count = 0;
			bool cancel = false;
			while (stream.total_in < (uInt)h.cSize && !cancel)
			{
				err = inflate(&stream, Z_SYNC_FLUSH);
				if (err == Z_STREAM_END)
				{
					err = Z_OK;
					break;
				}
				else if (err != Z_OK)
				{
					BGE_ASSERT(0 && "Something happened.");
					break;
				}

				stream.avail_out = (128 * 1024);
				stream.next_out += stream.total_out;

				progressCallback(count * 100 / h.cSize, cancel);
			}
			inflateEnd(&stream);
		}

		if (err != Z_OK)
		{
			bRet = false;
		}
		BGE_SAFE_DELETE_ARRAY(pcData);
		return bRet;
	}
} // End namespace (BGE)

/*
Example useage:

void MakePath(const char *pszPath)
{
  if (pszPath[0] == '\0')
    return;

  char buf[1000];
  const char *p = pszPath;

//  printf("MakePath(\"%s\")\n", pszPath);

  // Skip machine name in network paths like \\MyMachine\blah...
  if (p[0] == '\\' && p[1] == '\\')
    p = strchr(p+2, '\\');

  while (p != NULL && *p != '\0')
  {
    p = strchr(p, '\\');

    if (p)
    {
      memcpy(buf, pszPath, p - pszPath);
      buf[p - pszPath] = 0;
      p++;
    }
    else
      strcpy(buf, pszPath);

    if (buf[0] != '\0' && strcmp(buf, ".") && strcmp(buf, ".."))
    {
//      printf("  Making path: \"%s\"\n", buf);
      mkdir(buf);
    }
  }
}

void main(int argc, const char *argv[])
{
  if (argc > 1)
  {
    FILE *f = fopen(argv[1], "rb");
    if (f)
    {
      ZipFile zip;

      if (true != zip.Init(f))
        printf("Bad Zip file: \"%s\"\n", argv[1]);
      else
      {
        for (int i = 0; i < zip.GetNumFiles(); i++)
        {
          int len = zip.GetFileLen(i);
          char fname[1000];

          zip.GetFilename(i, fname);

          printf("File \"%s\" (%d bytes): ", fname, len);

          char *pData = GCC_NEW char[len];
          if (!pData)
            printf("OUT OF MEMORY\n");
          else if (true == zip.ReadFile(i, pData))
          {
            printf("OK\n");
            char dpath[1000];

            sprintf(dpath, "Data\\%s", fname);
            char *p = strrchr(dpath, '\\');
            if (p)
            {
              *p = '\0';
              MakePath(dpath);
              *p = '\\';
            }
            FILE *fo = fopen(dpath, "wb");
            if (fo)
            {
              fwrite(pData, len, 1, fo);
              fclose(fo);
            }
          }
          else
            printf("ERROR\n");
          delete[] pData;
        }
        zip.End();
      }

      fclose(f);
    }
  }
}
*/
