#ifndef _BGE_RESOURCEFILE_HPP_
#define _BGE_RESOURCEFILE_HPP_

#include "Resources/ZipFile.hpp"

namespace BGE
{
	class Resource; // Forward declare
	class IResourceFile; // Forward declare
	class ZipResourceFile; // Forward declare
	BGE_DECLARE_PTR(IResourceFile);
	BGE_DECLARE_PTR(ZipResourceFile);

	/**
	 * @brief .
	 */
	class IResourceFile
	{
	public:
		virtual ~IResourceFile(void) = default;

		virtual bool VOpen(void) = 0;
		virtual std::size_t VGetRawResourceSize(const Resource &kResource) = 0;
		virtual std::size_t VGetRawResource(const Resource &kResource, char *pBuffer) = 0;
		virtual std::size_t VGetNumResources(void) const = 0;
		virtual std::string VGetResourceName(std::size_t num) const = 0;
		virtual bool VIsUsingDevelopmentDirectories(void) const = 0;
	};

	/**
	 * @brief .
	 */
	class ZipResourceFile : public IResourceFile
	{
		UniqueZipFilePtr m_pZipFile;
		std::wstring m_fileName;
	public:
		ZipResourceFile(std::wstring_view fileName);
		virtual ~ZipResourceFile(void) = default;
		// IResourceFile's interface:
		virtual bool VOpen(void) override;
		virtual std::size_t VGetRawResourceSize(const Resource &kResource) override;
		virtual std::size_t VGetRawResource(const Resource &kResource, char *pBuffer) override;
		virtual std::size_t VGetNumResources(void) const override;
		virtual std::string VGetResourceName(std::size_t num) const override;
		virtual bool VIsUsingDevelopmentDirectories(void) const override;
	};

	/**
	 * @brief .
	 */
	class DevZipResourceFile : public ZipResourceFile
	{
	public:
		enum struct Mode
		{
			kDevelopment,
			kEditor
		};
	private:
		Mode m_mode;
		std::filesystem::path m_assetsPath;
		std::unordered_map<std::string, std::size_t> m_dirContentsMap;
		std::vector<std::filesystem::directory_entry> m_assetFileInfo;
	public:
		DevZipResourceFile(const std::filesystem::path &kAssetsPath, Mode mode);
		// IResourceFile's interface:
		virtual bool VOpen(void) override;
		virtual std::size_t VGetRawResourceSize(const Resource &kResource) override;
		virtual std::size_t VGetRawResource(const Resource &kResource, char *pBuffer) override;
		virtual std::size_t VGetNumResources(void) const override;
		virtual std::string VGetResourceName(std::size_t num) const override;
		virtual bool VIsUsingDevelopmentDirectories(void) const override;
	protected:
		void ReadAssetsDirectory(const std::filesystem::path &kPath);
		static std::string NormalizeResource(const Resource &kResource);
	};
} // End namespace (BGE)

#endif /* !_BGE_RESOURCEFILE_HPP_ */
