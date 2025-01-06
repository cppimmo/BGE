#include "Engine/EngineStd.hpp"
#include "Resources/ResourceLoader.hpp"

// Resource loaders:
#include "Resources/XMLResource.hpp"
#include "Scripting/ScriptResource.hpp"
#include "Graphics/ImageResource.hpp"
#include "Graphics/ShaderResource.hpp"
#include "Audio/SoundResource.hpp"
#include "Resources/JSONResource.hpp"
#include "Graphics/AssimpResource.hpp"

namespace BGE
{
	std::string DefaultResourceLoader::VGetPattern(void) const
	{
		return "";
	}

	ResourceType DefaultResourceLoader::VGetType(void) const
	{
		return ResourceType::kUnknown;
	}

	bool DefaultResourceLoader::VUseRawFile(void) const
	{
		return true;
	}

	bool DefaultResourceLoader::VDiscardRawBufferAfterLoad(void)
	{
		return true;
	}

	bool DefaultResourceLoader::VAddNullZero(void)
	{
		return false;
	}

	std::size_t DefaultResourceLoader::VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize)
	{
		return rawSize;
	}

	bool DefaultResourceLoader::VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle)
	{
		return true;
	}

	StrongIResourceLoaderPtr DefaultResourceLoaderFactory::VCreateDefaultResourceLoader(void)
	{
		return std::make_shared<DefaultResourceLoader>();
	}

	StrongIResourceLoaderPtr DefaultResourceLoaderFactory::VCreateXMLResourceLoader(void)
	{
		return std::make_shared<XMLResourceLoader>();
	}

	StrongIResourceLoaderPtr DefaultResourceLoaderFactory::VCreateScriptResourceLoader(void)
	{
		return std::make_shared<ScriptResourceLoader>();
	}

	StrongIResourceLoaderPtr DefaultResourceLoaderFactory::VCreateBMPResourceLoader(void)
	{
		return std::make_shared<BMPResourceLoader>();
	}

	StrongIResourceLoaderPtr DefaultResourceLoaderFactory::VCreateJPEGResourceLoader(void)
	{
		return std::make_shared<JPEGResourceLoader>();
	}

	StrongIResourceLoaderPtr DefaultResourceLoaderFactory::VCreatePNGResourceLoader(void)
	{
		return std::make_shared<PNGResourceLoader>();
	}

	StrongIResourceLoaderPtr DefaultResourceLoaderFactory::VCreateTGAResourceLoader(void)
	{
		return std::make_shared<TGAResourceLoader>();
	}

	StrongIResourceLoaderPtr DefaultResourceLoaderFactory::VCreateGLSLResourceLoader(void)
	{
		return std::make_shared<GLSLResourceLoader>();
	}

	StrongIResourceLoaderPtr DefaultResourceLoaderFactory::VCreateWAVResourceLoader(void)
	{
		return std::make_shared<WAVResourceLoader>();
	}

	StrongIResourceLoaderPtr DefaultResourceLoaderFactory::VCreateOGGResourceLoader(void)
	{
		return std::make_shared<OGGResourceLoader>();
	}

	StrongIResourceLoaderPtr DefaultResourceLoaderFactory::VCreateJSONResourceLoader(void)
	{
		return std::make_shared<JSONResourceLoader>();
	}

	StrongIResourceLoaderPtr DefaultResourceLoaderFactory::VCreateAssimpResourceLoader(void)
	{
		return std::make_shared<AssimpResourceLoader>();
	}
} // End namespace (BGE)
