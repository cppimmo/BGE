#include "Engine/EngineStd.hpp"
#include "Resources/XMLResourceLoader.hpp"

std::string BGE::XMLResourceExtraData::VGetExtraData(void)
{
	return "XMLResourceExtraData";
}

void BGE::XMLResourceExtraData::ParseXML(char* pRawBuffer)
{
	m_document.Parse(pRawBuffer);
}

tinyxml2::XMLElement *BGE::XMLResourceExtraData::GetRoot(void)
{
	return m_document.RootElement();
}

const std::string &BGE::XMLResourceLoader::VGetPattern(void) const
{
	return m_pattern;
}

bool BGE::XMLResourceLoader::VUseRawFile(void) const
{
	return false;
}

bool BGE::XMLResourceLoader::VDiscardRawBufferAfterLoad(void)
{
	return true;
}

bool BGE::XMLResourceLoader::VAddNullZero(void)
{
	return true;
}

std::size_t BGE::XMLResourceLoader::VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize)
{
	return rawSize;
}

bool BGE::XMLResourceLoader::VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle)
{
	if (size <= 0)
	{
		return false;
	}

	auto pExtraData = std::make_shared<XMLResourceExtraData>();
	pExtraData->ParseXML(pRawBuffer);

	pResourceHandle->SetExtraData(pExtraData);
	return true;
}

tinyxml2::XMLElement *BGE::XMLResourceLoader::LoadAndReturnRootXMLElement(std::string_view resourceName)
{
	const Resource kResource(resourceName);
	// Load XML file from the resource archive
	auto &app = GetEngineApp();
	auto pResourceHandle = app.GetResourceCache().GetHandle(kResource);
	auto pExtraData = std::dynamic_pointer_cast<XMLResourceExtraData>(pResourceHandle->GetExtraData());
	return pExtraData->GetRoot();
}
