#include "Engine/EngineStd.hpp"
#include "Resources/JSONResource.hpp"

namespace BGE
{
	JSONResourceExtraData::JSONResourceExtraData(const json &data)
		: m_json(data)
	{
	}

	std::string JSONResourceExtraData::VGetExtraData(void)
	{
		return m_json.dump();
	}

	const JSONResourceExtraData::json &JSONResourceExtraData::GetJSON(void) const
	{
		return m_json;
	}

	const std::string &JSONResourceLoader::VGetPattern(void) const
	{
		return m_pattern;
	}

	bool JSONResourceLoader::VUseRawFile(void) const
	{
		return false;
	}

	bool JSONResourceLoader::VDiscardRawBufferAfterLoad(void)
	{
		return true;
	}

	bool JSONResourceLoader::VAddNullZero(void)
	{
		return true;
	}

	std::size_t JSONResourceLoader::VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize)
	{
		return rawSize;
	}

	bool JSONResourceLoader::VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle)
	{
		if (!pRawBuffer || size <= 0)
			return false;

		using json = JSONResourceExtraData::json;
		try
		{
			// Attempt to parse the JSON data from the buffer
			auto parsedJson = json::parse(pRawBuffer);

			// Create and attach the extra data
			auto pExtraData = std::make_shared<JSONResourceExtraData>(parsedJson);
			pResourceHandle->SetExtraData(pExtraData);

			return true;
		}
		catch (const json::parse_error &ex)
		{
			// Log parse error details
			BGE_ERROR("JSON parse error at byte %d: %s", ex.byte, ex.what());
		}
		catch (const json::exception &ex)
		{
			// Catch other JSON exceptions
			BGE_ERROR("JSON error: %s", ex.what());
		}
		catch (const std::exception &ex)
		{
			// Handle other standard exceptions (e.g., std::bad_alloc)
			BGE_ERROR("Standard error: %s", ex.what());
		}
		catch (...)
		{
			// Catch-all for unknown exceptions
			BGE_ERROR("An unknown error occurred during JSON parsing.");
		}
		return false; // Return false if any error occurs
	}
} // End namespace (BGE)
