#ifndef _BGE_RESOURCELOADER_HPP_
#define _BGE_RESOURCELOADER_HPP_

#include "Resources/ResourceHandle.hpp"

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
		virtual StrongDefaultResourceLoaderPtr VCreateDefaultResourceLoader(void) const = 0;
	};
} // End namespace (BGE)

#endif /* !_BGE_RESOURCELOADER_HPP_ */
