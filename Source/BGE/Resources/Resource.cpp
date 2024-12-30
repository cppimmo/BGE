#include "Engine/EngineStd.hpp"
#include "Resources/Resource.hpp"

namespace BGE
{
	PlaintextExtraData::PlaintextExtraData(std::string_view plaintext)
		: m_plaintext(plaintext)
	{
	}

	std::string PlaintextExtraData::VGetExtraData(void)
	{
		return m_plaintext;
	}
} // End namespace (BGE)
