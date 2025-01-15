#include "Engine/EngineStd.hpp"
#include "Graphics/ShaderResource.hpp"

namespace BGE
{
	std::string GLSLResourceLoader::VGetPattern(void) const
	{
		return "*.glsl";
	}

	ResourceType GLSLResourceLoader::VGetType(void) const
	{
		return ResourceType::kGLSL;
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
		auto pExtraData = std::make_shared<PlaintextResourceExtraData>(pRawBuffer);
		pResourceHandle->SetExtraData(pExtraData);
		return true;
	}

	std::string HLSLResourceLoader::VGetPattern(void) const
	{
		return "*.hlsl";
	}

	ResourceType HLSLResourceLoader::VGetType(void) const
	{
		return ResourceType::kHLSL;
	}

	bool HLSLResourceLoader::VUseRawFile(void) const
	{
		return false;
	}

	bool HLSLResourceLoader::VDiscardRawBufferAfterLoad(void)
	{
		return true;
	}

	bool HLSLResourceLoader::VAddNullZero(void)
	{
		return true;
	}

	std::size_t HLSLResourceLoader::VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize)
	{
		return rawSize;
	}

	bool HLSLResourceLoader::VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle)
	{
		if (size <= 0)
			return false;

		// Set the shader plaintext source code extra data
		auto pExtraData = std::make_shared<PlaintextResourceExtraData>(pRawBuffer);
		pResourceHandle->SetExtraData(pExtraData);
		return true;
	}

	std::string SPIRVResourceLoader::VGetPattern(void) const
	{
		// TODO: Add other accepted SPIR-V extensions.
		return "*.spv";
	}

	ResourceType SPIRVResourceLoader::VGetType(void) const
	{
		return ResourceType::kSPIRV;
	}

	bool SPIRVResourceLoader::VUseRawFile(void) const
	{
		return false;
	}

	bool SPIRVResourceLoader::VDiscardRawBufferAfterLoad(void)
	{
		return true;
	}

	bool SPIRVResourceLoader::VAddNullZero(void)
	{
		return true;
	}

	std::size_t SPIRVResourceLoader::VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize)
	{
		return rawSize;
	}

	bool SPIRVResourceLoader::VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle)
	{
		if (size <= 0)
			return false;

		// TODO: Implement this resource loader.
		// Set the shader plaintext source code extra data
		//auto pExtraData = std::make_shared<PlaintextResourceExtraData>(pRawBuffer);
		//pResourceHandle->SetExtraData(pExtraData);
		return true;
	}
} // End namespace (BGE)
