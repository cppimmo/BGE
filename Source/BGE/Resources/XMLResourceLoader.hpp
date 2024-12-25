#ifndef _BGE_XMLRESOURCELOADER_HPP_
#define _BGE_XMLRESOURCELOADER_HPP_

#include "Resources/Resource.hpp"
#include "Resources/ResourceLoader.hpp"

#include <tinyxml2.h>

namespace BGE
{
	class XMLResourceExtraData; // Forward declare
	BGE_DECLARE_PTR(XMLResourceExtraData);
	class XMLResourceLoader; // Forward declare
	BGE_DECLARE_PTR(XMLResourceLoader);

	/**
	 * @brief .
	 */
	class XMLResourceExtraData : public IResourceExtraData
	{
	protected:
		tinyxml2::XMLDocument m_document;
	public:
		// IResourceExtraData's interface:
		virtual std::string VGetExtraData(void) override;

		void ParseXML(char *pRawBuffer);
		tinyxml2::XMLElement *GetRoot(void);
	};

	/**
	 * @brief .
	 */
	class XMLResourceLoader : public IResourceLoader
	{
		std::string m_pattern = "*.xml";
	public:
		// IResourceLoader's interface:
		virtual const std::string &VGetPattern(void) const override;
		virtual bool VUseRawFile(void) const override;
		virtual bool VDiscardRawBufferAfterLoad(void) override;
		virtual bool VAddNullZero(void) override;
		virtual std::size_t VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize) override;
		virtual bool VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle) override;
	private:
		static tinyxml2::XMLElement *LoadAndReturnRootXMLElement(std::string_view resourceName);
	};
} // End namespace (BGE)

#endif /* !_BGE_XMLRESOURCELOADER_HPP_ */
