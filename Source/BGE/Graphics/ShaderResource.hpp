#ifndef _BGE_SHADERRESOURCE_HPP_
#define _BGE_SHADERRESOURCE_HPP_

#include "Resources/ResourceLoader.hpp"

namespace BGE
{
	class GLSLResourceLoader; // Forward declare
	BGE_DECLARE_PTR(GLSLResourceLoader);

	/**
	 * @brief Resource loader for .glsl shader source code files.
	 */
	class GLSLResourceLoader final : public IResourceLoader
	{
	public:
		GLSLResourceLoader(void) = default;
		// IResourceLoader's interface:
		virtual std::string VGetPattern(void) const override;
		virtual ResourceType VGetType(void) const override;
		virtual bool VUseRawFile(void) const override;
		virtual bool VDiscardRawBufferAfterLoad(void) override;
		virtual bool VAddNullZero(void) override;
		virtual std::size_t VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize) override;
		virtual bool VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle) override;
	};

	/**
	 * @brief Resource loader for .spv shader source code files.
	 */
	class SPIRVResourceLoader final : public IResourceLoader
	{
	public:
		SPIRVResourceLoader(void) = default;
		// IResourceLoader's interface:
		virtual std::string VGetPattern(void) const override;
		virtual ResourceType VGetType(void) const override;
		virtual bool VUseRawFile(void) const override;
		virtual bool VDiscardRawBufferAfterLoad(void) override;
		virtual bool VAddNullZero(void) override;
		virtual std::size_t VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize) override;
		virtual bool VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle) override;
	};
} // End namespace (BGE)

#endif /* !_BGE_SHADERRESOURCE_HPP_ */
