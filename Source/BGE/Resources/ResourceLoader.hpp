#ifndef _BGE_RESOURCELOADER_HPP_
#define _BGE_RESOURCELOADER_HPP_

#include "Resources/ResourceHandle.hpp"
// Individual loaders:
//#include "Scripting/ScriptResourceLoader.hpp"

namespace BGE
{
	class IResourceLoader; // Forward declare
	BGE_DECLARE_PTR(IResourceLoader);
	class IResourceLoaderFactory; // Forward declare
	BGE_DECLARE_PTR(IResourceLoaderFactory);
	class DefaultResourceLoader; // Forward declare
	BGE_DECLARE_PTR(DefaultResourceLoader);

	//! .
	using ResourceLoaderList = std::list<StrongIResourceLoaderPtr>;

	/**
	 * @brief .
	 */
	class IResourceLoader
	{
	public:
		virtual ~IResourceLoader(void) = default;

		virtual const std::string &VGetPattern(void) const = 0;
		virtual bool VUseRawFile(void) const = 0;
		virtual bool VDiscardRawBufferAfterLoad(void) = 0;
		virtual bool VAddNullZero(void) = 0;
		virtual std::size_t VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize) = 0;
		virtual bool VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle) = 0;
	};

	/**
	 * @brief .
	 */
	class DefaultResourceLoader final : public IResourceLoader
	{
		std::string m_pattern;
	public:
		DefaultResourceLoader(void);
		// IResourceLoader's interface:
		virtual const std::string &VGetPattern(void) const override;
		virtual bool VUseRawFile(void) const override;
		virtual bool VDiscardRawBufferAfterLoad(void) override;
		virtual bool VAddNullZero(void) override;
		virtual std::size_t VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize) override;
		virtual bool VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle) override;
	};

	/**
	 * @brief .
	 */
	class IResourceLoaderFactory
	{
	public:
		virtual ~IResourceLoaderFactory(void) = default;
		// Interface:
		virtual StrongIResourceLoaderPtr VCreateDefaultResourceLoader(void) = 0;
		virtual StrongIResourceLoaderPtr VCreateXMLResourceLoader(void) = 0;
		virtual StrongIResourceLoaderPtr VCreateScriptResourceLoader(void) = 0;
		virtual StrongIResourceLoaderPtr VCreateBMPResourceLoader(void) = 0;
		virtual StrongIResourceLoaderPtr VCreateJPEGResourceLoader(void) = 0;
		virtual StrongIResourceLoaderPtr VCreatePNGResourceLoader(void) = 0;
		virtual StrongIResourceLoaderPtr VCreateTGAResourceLoader(void) = 0;
	};

	class DefaultResourceLoaderFactory : public IResourceLoaderFactory
	{
	public:
		virtual ~DefaultResourceLoaderFactory(void) = default;
		// IResourceLoaderFactory's interface:
		virtual StrongIResourceLoaderPtr VCreateDefaultResourceLoader(void) override;
		virtual StrongIResourceLoaderPtr VCreateXMLResourceLoader(void) override;
		virtual StrongIResourceLoaderPtr VCreateScriptResourceLoader(void) override;
		virtual StrongIResourceLoaderPtr VCreateBMPResourceLoader(void) override;
		virtual StrongIResourceLoaderPtr VCreateJPEGResourceLoader(void) override;
		virtual StrongIResourceLoaderPtr VCreatePNGResourceLoader(void) override;
		virtual StrongIResourceLoaderPtr VCreateTGAResourceLoader(void) override;
	};
} // End namespace (BGE)

#endif /* !_BGE_RESOURCELOADER_HPP_ */
