#include "Engine/EngineStd.hpp"
#include "Resources/ResourceFile.hpp"

namespace BGE
{
	ZipResourceFile::ZipResourceFile(std::wstring_view fileName)
		: m_pZipFile(nullptr), m_fileName(fileName)
	{
	}

	bool ZipResourceFile::VOpen(void)
	{
		m_pZipFile = std::make_unique<ZipFile>();
		if (m_pZipFile)
		{
			return m_pZipFile->Init(m_fileName);
		}
		return false;
	}

	std::size_t ZipResourceFile::VGetRawResourceSize(const Resource &kResource)
	{
		auto resourceNum = m_pZipFile->Find(kResource.GetName());
		if (resourceNum < 0)
		{
			return 0;
		}
		return m_pZipFile->GetFileLen(resourceNum);
	}

	std::size_t ZipResourceFile::VGetRawResource(const Resource &kResource, char *pBuffer)
	{
		std::size_t size = 0;
		std::optional<BGE::ZipFileIndex> resourceNum = m_pZipFile->Find(kResource.GetName());
		if (resourceNum)
		{
			size = m_pZipFile->GetFileLen(*resourceNum);
			m_pZipFile->ReadFile(*resourceNum, pBuffer);
		}
		return size;
	}

	std::size_t ZipResourceFile::VGetNumResources(void) const
	{
		return (m_pZipFile ? m_pZipFile->GetNumFiles() : 0);
	}

	std::string ZipResourceFile::VGetResourceName(std::size_t num) const
	{
		std::string resourceName = "";
		if (m_pZipFile && num >= 0 && num < m_pZipFile->GetNumFiles())
		{
			resourceName = m_pZipFile->GetFileName(num);
		}
		return resourceName;
	}

	bool ZipResourceFile::VIsUsingDevelopmentDirectories(void) const
	{
		return false;
	}
} // End namespace (BGE)
