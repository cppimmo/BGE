#include "Engine/EngineStd.hpp"
#include "Resources/ResourceFile.hpp"

#include <fstream>

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

	DevZipResourceFile::DevZipResourceFile(const fs::path &kAssetsPath, Mode mode)
		: ZipResourceFile(kAssetsPath.wstring()), m_mode(mode)
	{
		// Initialize the assets directory
		fs::path tempPath = kAssetsPath;
		// Assume ZIP path points to the root of the assets
		tempPath.replace_extension(); // Remove the ZIP file extension ".zip"
		
		// Look for the assets prefaced with the current directory
		m_assetsPath = fs::current_path() / tempPath;
	}
	
	bool DevZipResourceFile::VOpen(void)
	{
		if (m_mode != Mode::kEditor)
		{
			ZipResourceFile::VOpen();
		}

		// Open the asset directory and read the visible (non-hidden contents)
		if (m_mode == Mode::kEditor)
		{
			ReadAssetsDirectory(m_assetsPath);
		}
		else
		{
			BGE_ASSERT(false && "Only Editor mode is supported for now.");
		}

		return true;
	}
	
	std::size_t DevZipResourceFile::VGetRawResourceSize(const Resource &kResource)
	{
		auto it = m_dirContentsMap.find(NormalizeResource(kResource));
		if (it == m_dirContentsMap.end())
		{
			return 0;
		}

		const auto &kEntry = m_assetFileInfo[it->second];
		return static_cast<std::size_t>(std::filesystem::file_size(kEntry));
	}
	
	std::size_t DevZipResourceFile::VGetRawResource(const Resource &kResource, char *pBuffer)
	{
		auto it = m_dirContentsMap.find(NormalizeResource(kResource));
		if (it == m_dirContentsMap.end())
		{
			return 0;
		}

		const auto &kEntry = m_assetFileInfo[it->second];
		std::ifstream file(kEntry.path(), std::ios::binary);
		if (!file)
		{
			return 0;
		}

		file.read(pBuffer, std::filesystem::file_size(kEntry));
		return static_cast<std::size_t>(file.gcount());
	}
	
	std::size_t DevZipResourceFile::VGetNumResources(void) const
	{
		return m_assetFileInfo.size();
	}
	
	std::string DevZipResourceFile::VGetResourceName(std::size_t index) const
	{
		BGE_ASSERT(index < m_assetFileInfo.size());

		return m_assetFileInfo[index].path().filename().string();
	}
	
	bool DevZipResourceFile::VIsUsingDevelopmentDirectories(void) const
	{
		return true;
	}
	
	void DevZipResourceFile::ReadAssetsDirectory(const std::filesystem::path &kPath)
	{
		for (const auto &kEntry : std::filesystem::recursive_directory_iterator(kPath))
		{
			if (kEntry.is_regular_file())
			{
				auto relativePath = fs::relative(kEntry.path(), m_assetsPath.parent_path());

				m_dirContentsMap[relativePath.string()] = m_assetFileInfo.size();
				m_assetFileInfo.push_back(kEntry);
			}
		}
	}

	std::string DevZipResourceFile::NormalizeResource(const Resource &kResource)
	{
		/**
		 * Convert the resource name to an relative path to use the correct platform seperators.
		 */
		return fs::path(kResource.GetName()).string();
	}
} // End namespace (BGE)
