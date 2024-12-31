#ifndef _BGE_JSONRESOURCE_HPP_
#define _BGE_JSONRESOURCE_HPP_

#include "Resources/Resource.hpp"
#include "Resources/ResourceLoader.hpp"

#include <nlohmann/json.hpp>

namespace BGE
{
	class JSONResourceExtraData; // Forward declare
	BGE_DECLARE_PTR(JSONResourceExtraData);
	class JSONResourceLoader; // Forward declare
	BGE_DECLARE_PTR(JSONResourceLoader);

	/**
	 * @brief .
	 */
	class JSONResourceExtraData final : public IResourceExtraData
	{
	public:
		using json = nlohmann::json;
	private:
		json m_json;
	public:
		explicit JSONResourceExtraData(const json &data);

		//! Default destructor.
		virtual ~JSONResourceExtraData(void) override = default;

		// IResourceExtraData's interface:
		virtual std::string VGetExtraData(void) override;

		const json &GetJSON(void) const;
	};

	/**
	 * @brief .
	 */
	class JSONResourceLoader : public IResourceLoader
	{
		std::string m_pattern = "*.json";
	public:
		// IResourceLoader's interface:
		virtual const std::string &VGetPattern(void) const override;
		virtual bool VUseRawFile(void) const override;
		virtual bool VDiscardRawBufferAfterLoad(void) override;
		virtual bool VAddNullZero(void) override;
		virtual std::size_t VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize) override;
		virtual bool VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle) override;
	};
} // End namespace (BGE)

#endif /* !_BGE_JSONRESOURCE_HPP_ */
