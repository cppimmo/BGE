#include "Engine/EngineStd.hpp"
#include "Graphics/ShaderResource.hpp"

namespace BGE
{
	const std::string &GLSLResourceLoader::VGetPattern(void) const
	{
		return m_pattern;
	}

	bool GLSLResourceLoader::VUseRawFile(void) const
	{
		return false;
	}

	bool GLSLResourceLoader::VDiscardRawBufferAfterLoad(void)
	{
		return true;
	}

	bool GLSLResourceLoader::VAddNullZero(void)
	{
		return true;
	}

	std::size_t GLSLResourceLoader::VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize)
	{
		return rawSize;
	}

	bool GLSLResourceLoader::VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle)
	{
		if (size <= 0)
			return false;

		// Set the shader plaintext source code extra data
		auto pPlaintextExtraData = std::make_shared<PlaintextExtraData>(pRawBuffer);
		pResourceHandle->SetExtraData(pPlaintextExtraData);
		return true;
	}
} // End namespace (BGE)
