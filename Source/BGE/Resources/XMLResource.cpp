#include "Engine/EngineStd.hpp"
#include "Resources/XMLResource.hpp"

namespace BGE
{
	std::string XMLResourceExtraData::VGetExtraData(void)
	{
		return "XML_ResourceExtraData";
	}

	void XMLResourceExtraData::ParseXML(char* pRawBuffer)
	{
		m_document.Parse(pRawBuffer);
	}

	tinyxml2::XMLElement *XMLResourceExtraData::GetRoot(void)
	{
		return m_document.RootElement();
	}

	std::string XMLResourceLoader::VGetPattern(void) const
	{
		return "*.xml";
	}

	ResourceType XMLResourceLoader::VGetType(void) const
	{
		return ResourceType::kXML;
	}

	bool XMLResourceLoader::VUseRawFile(void) const
	{
		return false;
	}

	bool XMLResourceLoader::VDiscardRawBufferAfterLoad(void)
	{
		return true;
	}

	bool XMLResourceLoader::VAddNullZero(void)
	{
		return true;
	}

	std::size_t XMLResourceLoader::VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize)
	{
		return rawSize;
	}

	bool XMLResourceLoader::VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle)
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

	tinyxml2::XMLElement *XMLResourceLoader::LoadAndReturnRootXMLElement(std::string_view resourceName)
	{
		const Resource kResource(resourceName);
		// Load XML file from the resource archive
		auto &app = GetEngineApp();
		auto pResourceHandle = app.GetResourceCache().GetHandle(kResource);
		auto pExtraData = std::dynamic_pointer_cast<XMLResourceExtraData>(pResourceHandle->GetExtraData());
		return pExtraData->GetRoot();
	}
} // End namespace (BGE)
