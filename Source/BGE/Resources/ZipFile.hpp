/*******************************************************************************
 * @file   ZipFile.hpp
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
#ifndef _BGE_ZIPFILE_HPP_
#define _BGE_ZIPFILE_HPP_

#include <cstdio>

namespace BGE
{
	class ZipFile; // Forward declare
	BGE_DECLARE_PTR(ZipFile);

	using ZipFileIndex = std::int64_t;
	using ZipContentsMap = std::map<std::string, ZipFileIndex>; // Maps path to a zip content ID
	using ZipProgressCallback = std::function<void(int, bool &)>;

	/**
	 * @brief .
	 *
	 * Based on example from GCC4 and original author Javier Arevalo.
	 */
	class ZipFile : public INonCopyable, public INonMovable
	{
	public:
		ZipFile(void);
		~ZipFile(void);

		bool Init(const std::wstring &resFileName);
		void End(void);

		std::size_t GetNumFiles(void) const { return m_nEntries; }
		std::string GetFileName(ZipFileIndex index) const;
		std::size_t GetFileLen(ZipFileIndex index) const;
		bool ReadFile(ZipFileIndex index, void *pBuf);

		// Added to show multi-threaded decompression
		bool ReadLargeFile(ZipFileIndex index, void *pBuf, ZipProgressCallback progressCallback);

		ZipFileIndex Find(const std::string &path) const;

		ZipContentsMap m_ZipContentsMap;
	private:
		struct TZipDirHeader;
		struct TZipDirFileHeader;
		struct TZipLocalHeader;

		FILE *m_pFile;    // Zip file
		char *m_pDirData; // Raw data buffer.
		std::size_t m_nEntries;  // Number of entries.

		// Pointers to the dir entries in pDirData.
		const TZipDirFileHeader **m_papDir;
	};
} // End namespace (BGE)

#endif /* !_BGE_ZIPFILE_HPP_ */
