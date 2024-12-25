#include "Engine/EngineStd.hpp"
#include "Resources/ResourceLoader.hpp"

// Resource loaders:
#include "Resources/XMLResourceLoader.hpp"
#include "Scripting/ScriptResourceLoader.hpp"

BGE::DefaultResourceLoader::DefaultResourceLoader(void)
	: m_pattern("*")
{
}

const std::string &BGE::DefaultResourceLoader::VGetPattern(void) const
{
	return m_pattern;
}

bool BGE::DefaultResourceLoader::VUseRawFile(void) const
{
	return true;
}

bool BGE::DefaultResourceLoader::VDiscardRawBufferAfterLoad(void)
{
	return true;
}

bool BGE::DefaultResourceLoader::VAddNullZero(void)
{
	return false;
}

std::size_t BGE::DefaultResourceLoader::VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize)
{
	return rawSize;
}

bool BGE::DefaultResourceLoader::VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle)
{
	return true;
}

BGE::StrongIResourceLoaderPtr BGE::DefaultResourceLoaderFactory::VCreateDefaultResourceLoader(void)
{
	return std::make_shared<DefaultResourceLoader>();
}

BGE::StrongIResourceLoaderPtr BGE::DefaultResourceLoaderFactory::VCreateXMLResourceLoader(void)
{
	return std::make_shared<XMLResourceLoader>();
}

BGE::StrongIResourceLoaderPtr BGE::DefaultResourceLoaderFactory::VCreateScriptResourceLoader(void)
{
	return std::make_shared<ScriptResourceLoader>();
}
