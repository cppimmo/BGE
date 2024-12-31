#include "Engine/EngineStd.hpp"
#include "Resources/Resource.hpp"

namespace BGE
{
	PlaintextResourceExtraData::PlaintextResourceExtraData(std::string_view plaintext)
		: m_plaintext(plaintext)
	{
	}

	std::string PlaintextResourceExtraData::VGetExtraData(void)
	{
		return m_plaintext;
	}
} // End namespace (BGE)
