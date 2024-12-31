#ifndef _BGE_SCRIPTRESOURCE_HPP_
#define _BGE_SCRIPTRESOURCE_HPP_

#include "Resources/ResourceLoader.hpp"

namespace BGE
{
	class ScriptResourceLoader; // Forward declare
	BGE_DECLARE_PTR(ScriptResourceLoader);

	class ScriptResourceLoader : public IResourceLoader
	{
	public:
		ScriptResourceLoader(void) = default;
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

#endif /* !_BGE_SCRIPTRESOURCE_HPP_ */
